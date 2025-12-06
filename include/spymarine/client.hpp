#pragma once

#include "spymarine/buffer.hpp"
#include "spymarine/crc.hpp"
#include "spymarine/device2.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/parse_count_info.hpp"
#include "spymarine/parse_device2.hpp"
#include "spymarine/parse_message.hpp"
#include "spymarine/parse_sensor2.hpp"
#include "spymarine/sensor2.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <expected>
#include <limits>
#include <span>
#include <utility>

namespace spymarine {

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
      : _buffer{}, _tcp_socket{std::move(tcp_socket_)},
        _udp_socket{std::move(udp_socket_)} {}

  constexpr std::expected<count_info, error> request_count_info() noexcept {
    return request_message(message_type::count_information, {})
        .and_then(parse_count_info);
  }

  constexpr std::expected<device2, error>
  request_device(device_id id) noexcept {
    const auto data = std::to_array<uint8_t>(
        {0x00, 0x01, 0x00, 0x00, 0x00, id, 0xff, 0x01, 0x03, 0x00, 0x00, 0x00,
         0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff});

    return request_message(message_type::device_information, data)
        .and_then([](const message& message) -> std::expected<device2, error> {
          if (message.type() == message_type::device_information) {
            return parse_device2(message.values());
          } else {
            return std::unexpected{parse_error::invalid_device_message};
          }
        });
  }

  constexpr std::expected<sensor2, error>
  request_sensor(sensor_id id) noexcept {
    const auto data =
        std::to_array<uint8_t>({0x01, 0x01, 0x00, 0x00, 0x00, id, 0xff, 0x02,
                                0x01, 0x00, 0x00, 0x00, 0x00, 0xff});

    return request_message(message_type::sensor_information, data)
        .and_then([](const message& message) -> std::expected<sensor2, error> {
          if (message.type() == message_type::sensor_information) {
            return parse_sensor2(message.values());
          } else {
            return std::unexpected{parse_error::invalid_sensor_message};
          }
        });
  }

  constexpr std::expected<message_values_view, error>
  read_sensor_state() noexcept {
    // read messages until a sensor state message is found
    auto msg = read_message();
    while (msg.transform([](const auto& m) { return m.type(); }) !=
           message_type::sensor_state) {
      msg = read_message();
    }

    return msg.transform([](const auto m) { return m.values(); });
  }

private:
  constexpr std::expected<message, error>
  request_message(const message_type type,
                  const std::span<const uint8_t> data) noexcept {
    return _tcp_socket.send(write_message_data(type, data, _buffer))
        .and_then([&, this]() { return _tcp_socket.receive(_buffer); })
        .and_then([](const std::span<const uint8_t> dat) {
          return parse_message(dat);
        });
  }

  constexpr std::span<uint8_t>
  write_message_data(const message_type type,
                     const std::span<const uint8_t> data,
                     std::span<uint8_t> buffer) noexcept {
    const auto payload_size = header_size + data.size();
    const auto message_length = data.size() + 3;

    assert(buffer.size() >= payload_size + 2);
    assert(message_length < std::numeric_limits<uint16_t>::max());

    const auto lb =
        std::bit_cast<std::array<uint8_t, 2>>(uint16_t(message_length));
    const auto type_value = std::to_underlying(type);
    const auto header = std::array<uint8_t, header_size>{
        0x00, 0x00, 0x00, 0x00, 0x00,  0xff,  type_value,
        0x04, 0x8c, 0x55, 0x4b, lb[1], lb[0], 0xff};

    const auto copy_header = std::ranges::copy(header, buffer.begin());
    const auto copy_data = std::ranges::copy(data, copy_header.out);

    const auto cb = std::bit_cast<std::array<uint8_t, 2>>(
        crc(buffer.subspan(0, payload_size - 1)));
    const auto copy_crc =
        std::ranges::copy(cb | std::views::reverse, copy_data.out);

    return std::span{buffer.begin(), copy_crc.out};
  }

  constexpr std::expected<message, error> read_message() noexcept {
    return _udp_socket.receive(_buffer).and_then(
        [](const auto bytes) { return parse_message(bytes); });
  }

  static_buffer _buffer;
  tcp_socket_type _tcp_socket;
  udp_socket_type _udp_socket;
};

} // namespace spymarine
