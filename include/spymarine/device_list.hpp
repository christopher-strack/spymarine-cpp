#pragma once

#include "spymarine/device2.hpp"
#include "spymarine/device_info.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/sensor2.hpp"

#include <ranges>
#include <unordered_map>
#include <vector>

namespace spymarine {

template <typename T>
concept device_info_range =
    std::ranges::input_range<T> &&
    std::convertible_to<std::ranges::range_reference_t<T>, const device_info&>;

template <typename T>
concept sensor_value_range =
    std::ranges::input_range<T> &&
    std::convertible_to<std::ranges::range_reference_t<T>,
                        const numeric_value1&>;

class device_list {
public:
  template <device_info_range range>
  constexpr explicit device_list(range&& infos)
      : _devices{make_devices(std::forward<range>(infos))},
        _sensor_map{make_sensor_map(_devices)} {}

  device_list(const device_list&&) = delete;
  device_list& operator=(const device_list&&) = delete;

  template <sensor_value_range range>
  constexpr void update_sensor_values(range&& numbers) {
    for (const auto& num : numbers) {
      const auto id = num.id();

      if (auto it = _sensor_map.find(id); it != _sensor_map.end()) {
        set_sensor_value(it->second, num);
      }
    }
  }

  constexpr const std::vector<device2>& get() const noexcept {
    return _devices;
  }

private:
  template <device_info_range range>
  static constexpr std::vector<device2> make_devices(range&& infos) {
    std::vector<device2> devices;
    devices.reserve(infos.size());

    sensor_id cumulative_offset = 0;
    for (auto info : infos) {
      if (auto dev = make_device(std::move(info), cumulative_offset)) {
        devices.push_back(std::move(*dev));
      }
      cumulative_offset += sensor_id_offset(info);
    }

    return devices;
  }

  static constexpr std::unordered_map<sensor_id, any_sensor_ref>
  make_sensor_map(std::vector<device2>& devices) {
    std::unordered_map<sensor_id, any_sensor_ref> sensor_map;

    for (auto& dev : devices) {
      std::visit(overloaded{
                     [&sensor_map](battery_device2& d) {
                       sensor_map.emplace(d.charge_sensor.id(),
                                          std::ref(d.charge_sensor));
                       sensor_map.emplace(d.current_sensor.id(),
                                          std::ref(d.current_sensor));
                       sensor_map.emplace(d.voltage_sensor.id(),
                                          std::ref(d.voltage_sensor));
                     },
                     [&sensor_map](auto& d) {
                       sensor_map.emplace(d.sensor.id(), std::ref(d.sensor));
                     },
                 },
                 dev);
    }

    return sensor_map;
  }

  std::vector<device2> _devices;
  std::unordered_map<sensor_id, any_sensor_ref> _sensor_map;
};

} // namespace spymarine
