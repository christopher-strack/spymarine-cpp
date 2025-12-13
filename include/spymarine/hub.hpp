#pragma once

#include "spymarine/client.hpp"
#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/parse_error.hpp"
#include "spymarine/tcp_socket.hpp"
#include "spymarine/udp_socket.hpp"

#include <expected>
#include <vector>

namespace spymarine {

template <typename tcp_socket_type, typename udp_socket_type> class basic_hub {
public:
  constexpr basic_hub(basic_client<tcp_socket_type, udp_socket_type> client_,
                      system_info system_info_, std::vector<device> devices,
                      std::vector<sensor2> sensors,
                      message_values_view initial_sensor_values) noexcept
      : _client{std::move(client_)}, _system_info{std::move(system_info_)},
        _devices{std::move(devices)}, _sensors{std::move(sensors)} {
    update_sensor_values(_sensors, initial_sensor_values, _average_count);
  }

  void read_sensor_values() {
    const auto values = _client.read_sensor_state();
    update_sensor_values(_sensors, values, _average_count);
    _average_count++;
  }

  void start_new_average_window() noexcept { _average_count = 0; }

  const system_info& system() const noexcept { return _system_info; }

  const std::vector<device>& all_devices() const noexcept { return _devices; }

  auto devices() const noexcept {
    return _devices | std::views::filter([](const auto& device_) {
             return !std::holds_alternative<unsupported_device>(device_);
           });
  }

  const std::vector<sensor2>& all_sensors() const noexcept { return _sensors; }

  auto sensors() const noexcept {
    return _sensors | std::views::filter([](const auto& sensor_) {
             return !std::holds_alternative<unsupported_sensor2>(sensor_);
           });
  }

  template <typename sensor_type> auto sensors_by_type() const noexcept {
    return _sensors | std::views::filter([](const auto& s) {
             return std::holds_alternative<sensor_type>(s);
           }) |
           std::views::transform([](const auto& s) -> const sensor_type& {
             return std::get<sensor_type>(s);
           });
  }

  auto all_sensors(const device& device_) const noexcept {
    return get_sensors(device_, _sensors);
  }

  auto sensors(const device& device_) const noexcept {
    return get_sensors(device_, _sensors) |
           std::views::filter([](const auto& sensor_) {
             return !std::holds_alternative<unsupported_sensor2>(sensor_);
           });
  }

private:
  basic_client<tcp_socket_type, udp_socket_type> _client;
  system_info _system_info;
  std::vector<device> _devices;
  std::vector<sensor2> _sensors;
  size_t _average_count{};
};

template <typename tcp_socket_type, typename udp_socket_type>
basic_hub(tcp_socket_type&&, udp_socket_type&&)
    -> basic_hub<tcp_socket_type, udp_socket_type>;

using hub = basic_hub<tcp_socket, udp_socket>;

template <typename tcp_socket_type, typename udp_socket_type>
constexpr std::expected<basic_hub<tcp_socket_type, udp_socket_type>, error>
initialize_basic_hub(
    basic_client<tcp_socket_type, udp_socket_type> client_) noexcept {
  const auto system_response = client_.request_system_info();
  if (!system_response) {
    return std::unexpected{system_response.error()};
  }

  const auto count_response = client_.request_count_info();
  if (!count_response) {
    return std::unexpected{count_response.error()};
  }

  std::vector<device> devices;
  std::vector<sensor2> sensors;

  devices.reserve(count_response->device_count);
  sensors.reserve(count_response->sensor_count);

  for (const auto id :
       std::views::iota(device_id(0), count_response->device_count)) {
    auto device_ = client_.request_device(id);

    if (!device_) {
      return std::unexpected{device_.error()};
    }

    if (get_device_id(*device_) != id) {
      return std::unexpected{parse_error::invalid_device_message};
    }

    devices.push_back(std::move(*device_));
  }

  for (const auto id :
       std::views::iota(sensor_id(0), count_response->sensor_count)) {
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

  return basic_hub{std::move(client_), std::move(*system_response),
                   std::move(devices), std::move(sensors), *sensor_values};
}

inline std::expected<hub, error> initialize_hub(client client_) {
  return initialize_basic_hub(std::move(client_));
}

} // namespace spymarine
