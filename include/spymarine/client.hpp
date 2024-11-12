#pragma once

#include "parsing.hpp"
#include "spymarine/device.hpp"

#include <array>
#include <chrono>
#include <concepts>
#include <cstdint>
#include <optional>
#include <thread>
#include <variant>

namespace spymarine {

template <typename tcp_socket_type>
concept tcp_socket =
    requires(tcp_socket_type socket, uint32_t ip, uint16_t port) {
      { tcp_socket_type(ip, port) };
      { socket.send(std::span<uint8_t>{}) } -> std::same_as<bool>;
      {
        socket.receive(std::span<uint8_t>{})
      } -> std::same_as<std::optional<std::span<uint8_t>>>;
    };

template <typename container_type>
concept device_container = requires(container_type container) {
  container.insert(container.end(), device{});
};

namespace detail {
std::span<uint8_t> write_message_data(message m, std::span<uint8_t> buffer);
}

template <tcp_socket tcp_socket_type> class client {
public:
  client(uint32_t address, uint16_t port,
         const std::chrono::system_clock::duration request_limit =
             std::chrono::milliseconds{10})
      : _ip_address{address}, _port{port}, _request_limit{request_limit} {}

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

        if (!std::holds_alternative<null_device>(*device) &&
            !std::holds_alternative<unknown_device>(*device)) {
          devices.insert(devices.end(), std::move(*device));
        }
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
    wait_for_request_limit();

    tcp_socket_type socket{_ip_address, _port};
    if (socket.send(detail::write_message_data(m, _buffer))) {
      if (const auto raw_response = socket.receive(_buffer)) {
        return parse_message(*raw_response);
      }
    }
    return std::nullopt;
  }

  void wait_for_request_limit() {
    if (_last_request_time) {
      const auto delta = std::chrono::system_clock::now() - *_last_request_time;
      if (delta < _request_limit) {
        std::this_thread::sleep_for(_request_limit - delta);
      }
    }
  }

  uint32_t _ip_address;
  uint16_t _port;
  std::array<uint8_t, 1024> _buffer;
  std::chrono::system_clock::duration _request_limit;
  std::optional<std::chrono::system_clock::time_point> _last_request_time;
};

} // namespace spymarine
