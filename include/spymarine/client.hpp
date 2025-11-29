#pragma once

#include "spymarine/buffer.hpp"
#include "spymarine/crc.hpp"
#include "spymarine/device2.hpp"
#include "spymarine/device_info.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/parse_device_info.hpp"
#include "spymarine/parse_message.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <expected>
#include <limits>
#include <span>
#include <utility>
#include <variant>

namespace spymarine {

constexpr std::array<uint8_t, 2> to_bytes(uint16_t value) noexcept {
  return {uint8_t((value >> 8) & 0xff), uint8_t(value & 0xff)};
}

constexpr std::span<uint8_t>
write_message_data(const message_type type, const std::span<const uint8_t> data,
                   std::span<uint8_t> buffer) noexcept {
  const auto payload_size = header_size + data.size();
  const auto total_size = payload_size + 2;

  assert(buffer.size() >= total_size);
  assert(data.size() < std::numeric_limits<uint16_t>::max());

  const auto length = to_bytes(3 + uint16_t(data.size()));
  const auto type_value = std::to_underlying(type);
  const auto header = std::array<uint8_t, header_size>{
      0x00, 0x00, 0x00, 0x00, 0x00,      0xff,      type_value,
      0x04, 0x8c, 0x55, 0x4b, length[0], length[1], 0xff};

  auto it = std::copy(header.begin(), header.end(), buffer.begin());
  it = std::copy(data.begin(), data.end(), it);

  const auto calculated_crc =
      to_bytes(crc(std::span{buffer.begin() + 1, payload_size - 2}));
  std::copy(calculated_crc.begin(), calculated_crc.end(), it);

  return std::span{buffer.begin(), total_size};
}

constexpr std::expected<device_id, error>
parse_device_count(const message& msg) noexcept {
  if (msg.type() == message_type::device_count) {
    if (const auto count = msg.values().find<numeric_value1>(1)) {
      return count->uint32() + 1;
    }
  }
  return std::unexpected{parse_error::invalid_device_count_message};
}

template <typename tcp_socket_type>
concept tcp_socket_concept =
    requires(tcp_socket_type socket, uint32_t ip, uint16_t port) {
      {
        tcp_socket_type::open()
      } -> std::same_as<std::expected<tcp_socket_type, error>>;
      { socket.connect(ip, port) } -> std::same_as<std::expected<void, error>>;
      {
        socket.send(std::span<uint8_t>{})
      } -> std::same_as<std::expected<void, error>>;
      {
        socket.receive(std::span<uint8_t>{})
      } -> std::same_as<std::expected<std::span<const uint8_t>, error>>;
    };

template <typename udp_socket_type>
concept udp_socket_concept =
    requires(udp_socket_type socket, uint32_t ip, uint16_t port) {
      {
        socket.receive(std::span<uint8_t>{})
      } -> std::same_as<std::expected<std::span<const uint8_t>, error>>;
    };

template <tcp_socket_concept tcp_socket_type,
          udp_socket_concept udp_socket_type>
class client {
public:
  constexpr explicit client(tcp_socket_type&& tcp_socket_,
                            udp_socket_type&& udp_socket_) noexcept
      : _tcp_socket{std::move(tcp_socket_)},
        _udp_socket{std::move(udp_socket_)} {}

  constexpr std::expected<std::vector<device_info>, error>
  request_device_infos() noexcept {
    return request_device_info_count().and_then(
        [this](const device_id count)
            -> std::expected<std::vector<device_info>, error> {
          std::vector<device_info> infos;
          infos.reserve(count);

          for (const auto id : std::views::iota(0u, count)) {
            std::expected<device_info, error> info = request_device_info(id);

            if (!info) {
              return std::unexpected(info.error());
            }

            infos.emplace_back(std::move(*info));
          }

          return infos;
        });
  }

  constexpr std::expected<device_id, error>
  request_device_info_count() noexcept {
    return request_message(message_type::device_count, {})
        .and_then(parse_device_count);
  }

  constexpr std::expected<device_info, error>
  request_device_info(device_id id) noexcept {
    const auto id_bytes = std::bit_cast<std::array<uint8_t, 4>>(id);

    const auto data = std::to_array<uint8_t>(
        {0x00, 0x01, id_bytes[3], id_bytes[2], id_bytes[1], id_bytes[0], 0xff,
         0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
         0xff});

    return request_message(message_type::device_information, data)
        .and_then(
            [](const message& message) -> std::expected<device_info, error> {
              if (message.type() == message_type::device_information) {
                return parse_device_info(message.values());
              } else {
                return std::unexpected{parse_error::invalid_device_message};
              }
            });
  }

  constexpr auto read_sensor_values() noexcept {
    // read messages until a sensor state message is found
    auto msg = read_sensor_state();
    while (msg == std::unexpected(error{parse_error::unknown_message})) {
      msg = read_sensor_state();
    }

    return msg.transform([](const auto m) { return m.values(); })
        .transform([](const auto& values) {
          return values | std::views::filter([](const auto& val) {
                   return std::holds_alternative<numeric_value1>(val);
                 }) |
                 std::views::transform([](const auto& val) {
                   return std::get<numeric_value1>(val);
                 });
        });
  }

private:
  constexpr std::expected<message, error> read_sensor_state() noexcept {
    return _udp_socket.receive(_buffer)
        .and_then([](const auto bytes) { return parse_message(bytes); })
        .and_then([](const auto msg) -> std::expected<message, error> {
          if (msg.type() == message_type::sensor_state) {
            return std::expected<message, error>{msg};
          } else {
            return std::unexpected(error{parse_error::unknown_message});
          }
        });
  }

  constexpr std::expected<message, error>
  request_message(const message_type type,
                  const std::span<const uint8_t> data) noexcept {
    return _tcp_socket.send(write_message_data(type, data, _buffer))
        .and_then([&, this]() { return _tcp_socket.receive(_buffer); })
        .and_then([](const std::span<const uint8_t> dat) {
          return parse_message(dat);
        });
  }

  static_buffer _buffer;
  tcp_socket_type _tcp_socket;
  udp_socket_type _udp_socket;
};

} // namespace spymarine
