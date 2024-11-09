#pragma once

#include "parsing.hpp"

#include <array>
#include <concepts>
#include <optional>

namespace spymarine {

template <typename tcp_socket_type>
concept tcp_socket = requires(tcp_socket_type socket) {
  { socket.send(std::span<uint8_t>{}) } -> std::same_as<bool>;
  {
    socket.receive(std::span<uint8_t>{})
  } -> std::same_as<std::optional<std::span<uint8_t>>>;
};

template <typename container_type>
concept device_container = requires(container_type container) {
  container.insert(container.end(), device{});
};

std::span<uint8_t> write_data(message m, std::span<uint8_t> buffer);

template <tcp_socket tcp_socket_type> class client {
public:
  client(tcp_socket_type socket) : _socket{std::move(socket)} {}

  template <device_container container_type>
  bool read_devices(container_type& devices) {
    if (const auto device_count = request_device_count()) {
      return read_devices(*device_count, devices);
    }

    return false;
  }

private:
  template <device_container container_type>
  bool read_devices(const uint8_t device_count, container_type& devices) {
    if constexpr (requires(container_type c) { c.reserve(device_count); }) {
      devices.reserve(device_count);
    }

    uint8_t sensor_start_index = 0;

    for (uint8_t i = 0; i < device_count; i++) {
      if (auto device = request_device_info(i, sensor_start_index)) {
        sensor_start_index += sensor_state_offset(*device);
        devices.insert(devices.end(), std::move(*device));
      } else {
        return false;
      }
    }

    return true;
  }

  std::optional<uint8_t> request_device_count() {
    if (const auto m = request({message_type::device_count, {}})) {
      if (m->type == message_type::device_count && m->data.size() >= 6) {
        return m->data[5] + 1;
      }
    }
    return std::nullopt;
  }

  std::optional<device> request_device_info(uint8_t device_id,
                                            uint8_t sensor_start_index) {
    const std::array<uint8_t, 19> data{
        0x00, 0x01, 0x00, 0x00, 0x00, device_id, 0xff, 0x01, 0x03, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00,      0x00, 0x00, 0xff};
    if (const auto m = request({message_type::device_info, data})) {
      if (auto device = parse_device(m->data, sensor_start_index)) {
        return std::move(*device);
      }
    }
    return std::nullopt;
  }

  std::optional<message> request(const message& m) {
    if (_socket.send(write_data(m, _buffer))) {
      if (const auto raw_response = _socket.receive(_buffer)) {
        return parse_message(*raw_response);
      }
    }
    return std::nullopt;
  }

  tcp_socket_type _socket;
  std::array<uint8_t, 1024> _buffer;
};

} // namespace spymarine
