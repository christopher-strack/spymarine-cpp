#pragma once

#include <string>
#include <variant>

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

struct null_device {
  auto operator<=>(const null_device&) const = default;
};

struct unknown_device {
  auto operator<=>(const unknown_device&) const = default;
};

template <typename T, typename... Args> struct variant_extender;

template <typename... Args0, typename... Args1>
struct variant_extender<std::variant<Args0...>, Args1...> {
  using type = std::variant<Args0..., Args1...>;
};

using parsed_device =
    variant_extender<device, null_device, unknown_device>::type;

uint8_t sensor_state_offset(const parsed_device& device);

} // namespace spymarine
