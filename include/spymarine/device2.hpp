#pragma once

#include "spymarine/id.hpp"
#include "spymarine/rational.hpp"
#include "spymarine/unit.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace spymarine {

template <typename T, size_t Denominator, unit Unit> struct device_property {
  static constexpr auto unit = Unit;

  using value_type = rational<T, Denominator>;

  constexpr explicit device_property(T numerator) noexcept : value{numerator} {}

  rational<T, Denominator> value;

  auto operator<=>(const device_property&) const = default;
};

struct voltage_device2 {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const voltage_device2&) const = default;
};

struct current_device2 {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const current_device2&) const = default;
};

struct temperature_device2 {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const temperature_device2&) const = default;
};

struct barometer_device2 {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const barometer_device2&) const = default;
};

struct resistive_device2 {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const resistive_device2&) const = default;
};

enum class fluid_type {
  fresh_water,
  fuel,
  waste_water,
  unknown,
};

using tank_capacity = device_property<int16_t, 10, unit::liters>;

struct tank_device2 {
  device_id id;
  std::optional<std::string> name;
  std::optional<fluid_type> type;
  std::optional<tank_capacity> capacity;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const tank_device2&) const = default;
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

using battery_capacity = device_property<int16_t, 100, unit::amp_hours>;

struct battery_device2 {
  device_id id;
  std::optional<std::string> name;
  std::optional<battery_type> type;
  std::optional<battery_capacity> capacity;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const battery_device2&) const = default;
};

struct unsupported_device2 {
  device_id id;
  uint32_t raw_type;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const unsupported_device2&) const = default;
};

using device2 =
    std::variant<voltage_device2, current_device2, temperature_device2,
                 barometer_device2, resistive_device2, tank_device2,
                 battery_device2, unsupported_device2>;

constexpr void add_sensor_id(device2& device, sensor_id id) noexcept {
  std::visit([id](auto& dev) { dev.sensor_ids.push_back(id); }, device);
}

} // namespace spymarine
