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

struct sensor {
  sensor_type type;
  uint8_t state_index;
  float value;

  auto operator<=>(const sensor&) const = default;
};

struct pico_internal_device {
  sensor device_sensor;

  explicit pico_internal_device(uint8_t state_start_index);

  auto operator<=>(const pico_internal_device&) const = default;
};

struct voltage_device {
  std::string name;

  sensor device_sensor;

  voltage_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const voltage_device&) const = default;
};

struct current_device {
  std::string name;

  sensor device_sensor;

  current_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const current_device&) const = default;
};

struct temperature_device {
  std::string name;

  sensor device_sensor;

  temperature_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const temperature_device&) const = default;
};

struct barometer_device {
  std::string name;

  sensor device_sensor;

  barometer_device(std::string name, uint8_t state_start_index);

  auto operator<=>(const barometer_device&) const = default;
};

struct resistive_device {
  std::string name;

  sensor device_sensor;

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

  sensor volume_sensor;
  sensor level_sensor;

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

  sensor charge_sensor;
  sensor remaining_capacity_sensor;
  sensor current_sensor;
  sensor voltage_sensor;

  battery_device(std::string name, battery_type type, float capacity,
                 uint8_t state_start_index);

  auto operator<=>(const battery_device&) const = default;
};

using device =
    std::variant<pico_internal_device, voltage_device, current_device,
                 temperature_device, barometer_device, resistive_device,
                 tank_device, battery_device>;

using sensor_map = std::unordered_map<uint8_t, std::vector<sensor*>>;

template <typename T> sensor_map build_sensor_map(T& devices_range) {
  sensor_map result;

  const auto insert_sensor = [&result](sensor& s) {
    auto it = result.find(s.state_index);
    if (it == result.end()) {
      it = result.emplace(s.state_index, std::vector<sensor*>{}).first;
    }
    it->second.push_back(std::addressof(s));
  };

  for (device& device : devices_range) {
    std::visit(overloaded{
                   [&](auto& d) { insert_sensor(d.device_sensor); },
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

float sensor_value(numeric_value value, sensor_type type);

void update_sensor_states(message state_message, sensor_map& map);

} // namespace spymarine
