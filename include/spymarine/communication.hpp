#pragma once

#include "device.hpp"
#include "parsing.hpp"

namespace spymarine {

template <typename container_type>
concept device_container = requires(container_type container) {
  container.insert(container.end(), device{});
};

namespace detail {

template <typename client, device_container container_type>
bool read_devices(const uint8_t device_count, client& c,
                  std::span<uint8_t> request_buffer, container_type& devices) {
  uint8_t sensor_start_index = 0;
  std::array<uint8_t, 19> message_buffer;

  if constexpr (requires(container_type c) { c.reserve(device_count); }) {
    devices.reserve(device_count);
  }

  for (auto i = 0; i < device_count; i++) {
    const auto info_message = make_device_info_message(i, message_buffer);
    if (const auto response_message = c.request(info_message)) {
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

template <typename client_type, device_container container_type>
bool read_devices(client_type& client, std::span<uint8_t> request_buffer,
                  container_type& devices) {
  if (const auto response = client.request({message_type::device_count})) {
    if (const auto device_count = parse_device_count_message(*response)) {
      return detail::read_devices(*device_count, client, request_buffer,
                                  devices);
    }
  }

  return false;
}

} // namespace spymarine
