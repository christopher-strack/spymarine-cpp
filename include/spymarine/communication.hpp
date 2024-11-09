#pragma once

#include "client.hpp"
#include "device.hpp"
#include "parsing.hpp"

namespace spymarine {

template <typename container_type>
concept device_container = requires(container_type container) {
  container.insert(container.end(), device{});
};

namespace detail {

template <typename tcp_socket_type, device_container container_type>
bool read_devices(const uint8_t device_count, client<tcp_socket_type>& client,
                  std::span<uint8_t> request_buffer, container_type& devices) {
  uint8_t sensor_start_index = 0;
  std::array<uint8_t, 19> message_buffer;

  if constexpr (requires(container_type c) { c.reserve(device_count); }) {
    devices.reserve(device_count);
  }

  for (uint8_t i = 0; i < device_count; i++) {
    if (const auto response_message = client.request(device_info_request{i})) {
      if (auto device =
              parse_device(response_message->data, sensor_start_index)) {
        devices.insert(devices.end(), std::move(*device));
        sensor_start_index += sensor_state_offset(*device);
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  return true;
}
} // namespace detail

template <typename tcp_socket_type, device_container container_type>
bool read_devices(client<tcp_socket_type>& client,
                  std::span<uint8_t> request_buffer, container_type& devices) {
  if (const auto response = client.request(device_count_request{})) {
    if (const auto device_count = parse_device_count_message(*response)) {
      return detail::read_devices(*device_count, client, request_buffer,
                                  devices);
    }
  }

  return false;
}

} // namespace spymarine
