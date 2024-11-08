#pragma once

#include <string>

namespace spymarine {

struct null_device {
  static constexpr auto sensor_state_offset = 0;

  auto operator<=>(const null_device&) const = default;
};

struct pico_internal_device {
  static constexpr auto sensor_state_offset = 6;

  auto operator<=>(const pico_internal_device&) const = default;
};

struct voltage_device {
  static constexpr auto sensor_state_offset = 1;

  std::string name;

  auto operator<=>(const voltage_device&) const = default;
};

struct current_device {
  static constexpr auto sensor_state_offset = 2;

  std::string name;

  auto operator<=>(const current_device&) const = default;
};

struct temperature_device {
  static constexpr auto sensor_state_offset = 1;

  std::string name;

  auto operator<=>(const temperature_device&) const = default;
};

struct barometer_device {
  static constexpr auto sensor_state_offset = 2;

  std::string name;

  auto operator<=>(const barometer_device&) const = default;
};

struct resistive_device {
  static constexpr auto sensor_state_offset = 1;

  std::string name;

  auto operator<=>(const resistive_device&) const = default;
};

enum class fluid_type {
  fresh_water,
  fuel,
  waste_water,
  unknown,
};

struct tank_device {
  static constexpr auto sensor_state_offset = 1;

  std::string name;
  fluid_type type;
  float capacity;

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
  static constexpr auto sensor_state_offset = 5;

  std::string name;
  battery_type type;
  float capacity;

  auto operator<=>(const battery_device&) const = default;
};

struct unknown_device {
  static constexpr auto sensor_state_offset = 1;

  auto operator<=>(const unknown_device&) const = default;
};

using device =
    std::variant<null_device, pico_internal_device, voltage_device,
                 current_device, temperature_device, barometer_device,
                 resistive_device, tank_device, battery_device, unknown_device>;

size_t sensor_state_offset(const device& device);

} // namespace spymarine
