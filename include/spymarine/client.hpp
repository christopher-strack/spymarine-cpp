#pragma once

#include "spymarine/buffer.hpp"
#include "spymarine/crc.hpp"
#include "spymarine/defaults.hpp"
#include "spymarine/device2.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/parse_count_info.hpp"
#include "spymarine/parse_device2.hpp"
#include "spymarine/parse_message.hpp"
#include "spymarine/parse_sensor2.hpp"
#include "spymarine/parse_system_info.hpp"
#include "spymarine/sensor2.hpp"
#include "spymarine/tcp_socket.hpp"
#include "spymarine/udp_socket.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <expected>
#include <limits>
#include <span>
#include <utility>

namespace spymarine {

template <typename tcp_socket_type, typename udp_socket_type> class client {
public:
  constexpr explicit client(tcp_socket_type&& tcp_socket_,
                            udp_socket_type&& udp_socket_) noexcept
      : _buffer{}, _tcp_socket{std::move(tcp_socket_)},
        _udp_socket{std::move(udp_socket_)} {}

  constexpr std::expected<system_info, error> request_system_info() noexcept {
    return request_message(message_type::system_information, {})
        .and_then(parse_system_info);
  }

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
  request_sensor_state() noexcept {
    return request_message(message_type::sensor_state, {})
        .transform([](const message& msg) { return msg.values(); });
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

template <typename udp_socket_type>
std::expected<uint32_t, error>
discover_with_socket(uint16_t port = simarine_default_udp_port) {
  return udp_socket_type::open().and_then([port](auto udp_socket) {
    return udp_socket.bind(0, port).and_then(
        [&]() { return udp_socket.discover(); });
  });
}

inline std::expected<uint32_t, error>
discover(uint16_t port = simarine_default_udp_port) {
  return discover_with_socket<udp_socket>(port);
}

template <typename tcp_socket_type, typename udp_socket_type>
constexpr static std::expected<client<tcp_socket_type, udp_socket_type>, error>
connect_with_sockets(
    const uint32_t ip, const uint16_t udp_port = simarine_default_udp_port,
    const uint16_t tcp_port = simarine_default_tcp_port) noexcept {
  auto udp_socket_ = udp_socket_type::open();
  if (!udp_socket_) {
    return std::unexpected{udp_socket_.error()};
  }

  auto tcp_socket_ = tcp_socket_type::open();
  if (!tcp_socket_) {
    return std::unexpected{tcp_socket_.error()};
  }

  const auto connect_result = udp_socket_->bind(0, udp_port).and_then([&]() {
    return tcp_socket_->connect(ip, tcp_port);
  });

  if (!connect_result) {
    return std::unexpected{connect_result.error()};
  }

  return client{std::move(*tcp_socket_), std::move(*udp_socket_)};
}

inline std::expected<client<tcp_socket, udp_socket>, error>
connect(const uint32_t ip, const uint16_t udp_port = simarine_default_udp_port,
        const uint16_t tcp_port = simarine_default_tcp_port) noexcept {
  return connect_with_sockets<tcp_socket, udp_socket>(ip, udp_port, tcp_port);
}

} // namespace spymarine
