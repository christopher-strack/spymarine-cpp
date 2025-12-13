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
  constexpr basic_hub(basic_client<tcp_socket_type, udp_socket_type> cli,
                      system_info sys_info, std::vector<device> devices,
                      std::vector<sensor> sensors,
                      message_values_view initial_sensor_values) noexcept
      : _client{std::move(cli)}, _system_info{std::move(sys_info)},
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
    return _devices | std::views::filter([](const auto& dev) {
             return !std::holds_alternative<unsupported_device>(dev);
           });
  }

  const std::vector<sensor>& all_sensors() const noexcept { return _sensors; }

  auto sensors() const noexcept {
    return _sensors | std::views::filter([](const auto& sen) {
             return !std::holds_alternative<unsupported_sensor>(sen);
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

  auto all_sensors(const device& dev) const noexcept {
    return get_sensors(dev, _sensors);
  }

  auto sensors(const device& dev) const noexcept {
    return get_sensors(dev, _sensors) | std::views::filter([](const auto& sen) {
             return !std::holds_alternative<unsupported_sensor>(sen);
           });
  }

private:
  basic_client<tcp_socket_type, udp_socket_type> _client;
  system_info _system_info;
  std::vector<device> _devices;
  std::vector<sensor> _sensors;
  size_t _average_count{};
};

template <typename tcp_socket_type, typename udp_socket_type>
basic_hub(tcp_socket_type&&, udp_socket_type&&)
    -> basic_hub<tcp_socket_type, udp_socket_type>;

using hub = basic_hub<tcp_socket, udp_socket>;

template <typename tcp_socket_type, typename udp_socket_type>
constexpr std::expected<basic_hub<tcp_socket_type, udp_socket_type>, error>
initialize_basic_hub(
    basic_client<tcp_socket_type, udp_socket_type> cli) noexcept {
  const auto system_response = cli.request_system_info();
  if (!system_response) {
    return std::unexpected{system_response.error()};
  }

  const auto count_response = cli.request_count_info();
  if (!count_response) {
    return std::unexpected{count_response.error()};
  }

  std::vector<device> devices;
  std::vector<sensor> sensors;

  devices.reserve(count_response->device_count);
  sensors.reserve(count_response->sensor_count);

  for (const auto id :
       std::views::iota(device_id(0), count_response->device_count)) {
    auto dev = cli.request_device(id);

    if (!dev) {
      return std::unexpected{dev.error()};
    }

    if (get_device_id(*dev) != id) {
      return std::unexpected{parse_error::invalid_device_message};
    }

    devices.push_back(std::move(*dev));
  }

  for (const auto id :
       std::views::iota(sensor_id(0), count_response->sensor_count)) {
    auto sen = cli.request_sensor(id);

    if (!sen) {
      return std::unexpected{sen.error()};
    }

    if (get_sensor_id(*sen) != id) {
      return std::unexpected{parse_error::invalid_sensor_message};
    }

    if (const auto dev_id = get_parent_device_id(*sen);
        dev_id.has_value() && *dev_id < devices.size()) {
      add_sensor_id(devices[*dev_id], id);
    }

    sensors.push_back(std::move(*sen));
  }

  const auto sensor_values = cli.request_sensor_state();
  if (!sensor_values) {
    return std::unexpected{sensor_values.error()};
  }

  return basic_hub{std::move(cli), std::move(*system_response),
                   std::move(devices), std::move(sensors), *sensor_values};
}

inline std::expected<hub, error> initialize_hub(client cli) {
  return initialize_basic_hub(std::move(cli));
}

} // namespace spymarine
