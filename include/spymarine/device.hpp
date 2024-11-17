#pragma once

#include "spymarine/message.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/overloaded.hpp"

#include <expected>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace spymarine {

enum class sensor_type {
  volume,
  level,
  voltage,
  current,
  temperature,
  pressure,
  resistive,
  charge,
  capacity,
};

struct sensor_info {
  sensor_type type;
  uint8_t state_index;
  float value;

  auto operator<=>(const sensor_info&) const = default;
};

struct pico_internal_device {
  sensor_info voltage_sensor;

  explicit pico_internal_device(uint8_t state_start_index);

  auto operator<=>(const pico_internal_device&) const = default;
};

struct voltage_device {
  std::string name;

  sensor_info voltage_sensor;

  voltage_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const voltage_device&) const = default;
};

struct current_device {
  std::string name;

  sensor_info current_sensor;

  current_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const current_device&) const = default;
};

struct temperature_device {
  std::string name;

  sensor_info temperature_sensor;

  temperature_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const temperature_device&) const = default;
};

struct barometer_device {
  std::string name;

  sensor_info pressure_sensor;

  barometer_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const barometer_device&) const = default;
};

struct resistive_device {
  std::string name;

  sensor_info resistive_sensor;

  resistive_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const resistive_device&) const = default;
};

enum class fluid_type {
  fresh_water,
  fuel,
  waste_water,
  unknown,
};

struct tank_device {
  std::string name;
  fluid_type type;
  float capacity;

  sensor_info volume_sensor;
  sensor_info level_sensor;

  tank_device(std::string name, fluid_type type, float capacity,
              uint8_t state_start_index);

  auto operator<=>(const tank_device&) const = default;
};

enum class battery_type {
  wet_low_maintenance,
  wet_maintenance_free,
  agm,
  deep_cycle,
  gel,
  lifepo4,
  unknown,
};

struct battery_device {
  std::string name;
  battery_type type;
  float capacity;

  sensor_info charge_sensor;
  sensor_info remaining_capacity_sensor;
  sensor_info current_sensor;
  sensor_info voltage_sensor;

  battery_device(std::string name, battery_type type, float capacity,
                 uint8_t state_start_index);

  auto operator<=>(const battery_device&) const = default;
};

using device =
    std::variant<pico_internal_device, voltage_device, current_device,
                 temperature_device, barometer_device, resistive_device,
                 tank_device, battery_device>;

using sensor_map = std::unordered_map<uint8_t, std::vector<sensor_info*>>;

template <typename T> sensor_map build_sensor_map(T& devices_range) {
  sensor_map result;

  const auto insert_sensor = [&result](sensor_info& sensor) {
    auto it = result.find(sensor.state_index);
    if (it == result.end()) {
      it =
          result.emplace(sensor.state_index, std::vector<sensor_info*>{}).first;
    }
    it->second.push_back(std::addressof(sensor));
  };

  for (device& device : devices_range) {
    std::visit(
        overloaded{
            [](pico_internal_device&) {},
            [&](voltage_device& d) { insert_sensor(d.voltage_sensor); },
            [&](current_device& d) { insert_sensor(d.current_sensor); },
            [&](temperature_device& d) { insert_sensor(d.temperature_sensor); },
            [&](barometer_device& d) { insert_sensor(d.pressure_sensor); },
            [&](resistive_device& d) { insert_sensor(d.resistive_sensor); },
            [&](tank_device& d) {
              insert_sensor(d.level_sensor);
              insert_sensor(d.volume_sensor);
            },
            [&](battery_device& d) {
              insert_sensor(d.charge_sensor);
              insert_sensor(d.remaining_capacity_sensor);
              insert_sensor(d.current_sensor);
              insert_sensor(d.voltage_sensor);
            },
        },
        device);
  }

  return result;
}

inline float sensor_value(numeric_value value, sensor_type type) {
  switch (type) {
  case sensor_type::volume:
    return value.second() / 10.0;
  case sensor_type::level:
    return value.first() / 10.0;
  case sensor_type::voltage:
    return value.second() / 1000.0f;
  case sensor_type::current:
    return value.second() / 100.0f;
  case sensor_type::temperature:
    return value.second() / 10.0f;
  case sensor_type::pressure:
    return value.number();
  case sensor_type::resistive:
    return value.second();
  case sensor_type::charge:
    return value.first() / 160.0;
  case sensor_type::capacity:
    return value.second() / 100.0;
  }

  return 0.0f;
}

inline void update_sensor_states(message state_message, sensor_map& map) {
  if (state_message.type != message_type::sensor_state) {
    return;
  }

  message_values_view state_values{state_message.data};

  for (const auto& entry : state_values) {
    if (const auto value = std::get_if<numeric_value>(&entry.value)) {
      if (const auto it = map.find(entry.id); it != map.end()) {
        for (sensor_info* sensor : it->second) {
          sensor->value = sensor_value(*value, sensor->type);
        }
      }
    }
  }
}

} // namespace spymarine
