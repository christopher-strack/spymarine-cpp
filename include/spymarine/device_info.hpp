#pragma once

#include "spymarine/rational.hpp"
#include "spymarine/sensor2.hpp"

#include <string>
#include <string_view>
#include <variant>

namespace spymarine {

using device_id = uint32_t;

struct pico_internal_device_info {
  static constexpr sensor_id sensor_id_offset = 6;
  static constexpr std::string_view name = "Pico internal";

  device_id id;

  auto operator<=>(const pico_internal_device_info&) const = default;
};

struct voltage_device_info {
  static constexpr sensor_id sensor_id_offset = 1;

  device_id id;
  std::string name;

  auto operator<=>(const voltage_device_info&) const = default;
};

struct current_device_info {
  static constexpr sensor_id sensor_id_offset = 2;

  device_id id;
  std::string name;

  auto operator<=>(const current_device_info&) const = default;
};

struct temperature_device_info {
  static constexpr sensor_id sensor_id_offset = 1;

  device_id id;
  std::string name;

  auto operator<=>(const temperature_device_info&) const = default;
};

struct barometer_device_info {
  static constexpr sensor_id sensor_id_offset = 2;

  device_id id;
  std::string name;

  auto operator<=>(const barometer_device_info&) const = default;
};

struct resistive_device_info {
  static constexpr sensor_id sensor_id_offset = 1;

  device_id id;
  std::string name;

  auto operator<=>(const resistive_device_info&) const = default;
};

enum class fluid_type {
  fresh_water,
  fuel,
  waste_water,
  unknown,
};

using tank_capacity = rational<int16_t, 10, unit::liters>;

struct tank_device_info {
  static constexpr sensor_id sensor_id_offset = 1;

  device_id id;
  std::string name;
  fluid_type type;
  tank_capacity capacity;

  auto operator<=>(const tank_device_info&) const = default;
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

using battery_capacity = rational<int16_t, 100, unit::amp_hours>;

struct battery_device_info {
  static constexpr sensor_id sensor_id_offset = 5;

  device_id id;
  std::string name;
  battery_type type;
  battery_capacity capacity;

  auto operator<=>(const battery_device_info&) const = default;
};

struct null_device_info {
  static constexpr sensor_id sensor_id_offset = 0;

  device_id id;

  auto operator<=>(const null_device_info&) const = default;
};

struct unknown_device_info {
  static constexpr sensor_id sensor_id_offset = 1;

  device_id id;

  auto operator<=>(const unknown_device_info&) const = default;
};

using device_info =
    std::variant<pico_internal_device_info, voltage_device_info,
                 current_device_info, temperature_device_info,
                 barometer_device_info, resistive_device_info, tank_device_info,
                 battery_device_info, null_device_info, unknown_device_info>;

constexpr sensor_id sensor_id_offset(const device_info& info) noexcept {
  return std::visit([](const auto& i) { return i.sensor_id_offset; }, info);
}

} // namespace spymarine
