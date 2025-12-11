#pragma once

#include "spymarine/client.hpp"
#include "spymarine/device2.hpp"
#include "spymarine/error.hpp"
#include "spymarine/parse_error.hpp"
#include "spymarine/tcp_socket.hpp"
#include "spymarine/udp_socket.hpp"

#include <expected>
#include <vector>

namespace spymarine {

template <typename tcp_socket_type, typename udp_socket_type> class hub {
public:
  constexpr hub(client<tcp_socket_type, udp_socket_type> client_,
                std::vector<device2> devices, std::vector<sensor2> sensors,
                message_values_view initial_sensor_values) noexcept
      : _client{std::move(client_)}, _devices{std::move(devices)},
        _sensors{std::move(sensors)} {
    update_sensor_values(_sensors, initial_sensor_values, _average_count);
  }

  std::expected<void, error> read_sensor_values() {
    return _client.read_sensor_state().transform([this](const auto& values) {
      update_sensor_values(_sensors, values, _average_count);
      _average_count++;
    });
  }

  void start_new_average_window() noexcept { _average_count = 0; }

  const std::vector<device2>& devices() const noexcept { return _devices; }

  const std::vector<sensor2>& sensors() const noexcept { return _sensors; }

  auto sensors(const device2& device_) const noexcept {
    return get_sensors(device_, _sensors);
  }

private:
  client<tcp_socket_type, udp_socket_type> _client;
  std::vector<device2> _devices;
  std::vector<sensor2> _sensors;
  size_t _average_count{};
};

template <typename tcp_socket_type, typename udp_socket_type>
constexpr std::expected<hub<tcp_socket_type, udp_socket_type>, error>
initialize_hub_with_sockets(
    client<tcp_socket_type, udp_socket_type> client_) noexcept {
  const auto info = client_.request_count_info();
  if (!info) {
    return std::unexpected{info.error()};
  }

  std::vector<device2> devices;
  std::vector<sensor2> sensors;

  devices.reserve(info->device_count);
  sensors.reserve(info->sensor_count);

  for (const auto id : std::views::iota(device_id(0), info->device_count)) {
    auto device_ = client_.request_device(id);

    if (!device_) {
      return std::unexpected{device_.error()};
    }

    if (get_device_id(*device_) != id) {
      return std::unexpected{parse_error::invalid_device_message};
    }

    devices.push_back(std::move(*device_));
  }

  for (const auto id : std::views::iota(sensor_id(0), info->sensor_count)) {
    auto sensor_ = client_.request_sensor(id);

    if (!sensor_) {
      return std::unexpected{sensor_.error()};
    }

    if (get_sensor_id(*sensor_) != id) {
      return std::unexpected{parse_error::invalid_sensor_message};
    }

    if (const auto dev_id = get_parent_device_id(*sensor_);
        dev_id.has_value() && *dev_id < devices.size()) {
      add_sensor_id(devices[*dev_id], id);
    }

    sensors.push_back(std::move(*sensor_));
  }

  const auto sensor_values = client_.request_sensor_state();
  if (!sensor_values) {
    return std::unexpected{sensor_values.error()};
  }

  return hub{std::move(client_), std::move(devices), std::move(sensors),
             *sensor_values};
}

inline std::expected<hub<tcp_socket, udp_socket>, error>
initialize_hub(client<tcp_socket, udp_socket> client_) {
  return initialize_hub_with_sockets(std::move(client_));
}

} // namespace spymarine
