#pragma once

#include "spymarine/id.hpp"
#include "spymarine/rational.hpp"
#include "spymarine/sensor.hpp"
#include "spymarine/unit.hpp"

#include <cassert>
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

struct voltage_device {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const voltage_device&) const = default;
};

struct current_device {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const current_device&) const = default;
};

struct temperature_device {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const temperature_device&) const = default;
};

struct barometer_device {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const barometer_device&) const = default;
};

struct resistive_device {
  device_id id;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const resistive_device&) const = default;
};

enum class fluid_type {
  fresh_water,
  fuel,
  waste_water,
  unknown,
};

using tank_capacity = device_property<int16_t, 10, unit::liters>;

struct tank_device {
  device_id id;
  std::optional<std::string> name;
  std::optional<fluid_type> type;
  std::optional<tank_capacity> capacity;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const tank_device&) const = default;
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

struct battery_device {
  device_id id;
  std::optional<std::string> name;
  std::optional<battery_type> type;
  std::optional<battery_capacity> capacity;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const battery_device&) const = default;
};

struct unsupported_device {
  device_id id;
  uint32_t raw_type;
  std::optional<std::string> name;
  std::vector<sensor_id> sensor_ids = {};

  bool operator==(const unsupported_device&) const = default;
};

using device = std::variant<voltage_device, current_device, temperature_device,
                            barometer_device, resistive_device, tank_device,
                            battery_device, unsupported_device>;

constexpr device_id get_device_id(const device& device_) noexcept {
  return std::visit([](const auto& dev) { return dev.id; }, device_);
}

constexpr std::optional<std::string>
get_device_name(const device& device_) noexcept {
  return std::visit([](const auto& dev) { return dev.name; }, device_);
}

constexpr std::vector<sensor_id>
get_sensor_ids(const device& device_) noexcept {
  return std::visit([](const auto& dev) { return dev.sensor_ids; }, device_);
}

constexpr auto get_sensors(const device& device_,
                           const sensor_range auto& sensors) {
  return get_sensor_ids(device_) | std::views::filter([&](const auto id) {
           return id < std::ranges::size(sensors);
         }) |
         std::views::transform([&](const auto id) {
           const auto& sen = sensors[id];
           assert(get_sensor_id(sen) == id);
           return sen;
         });
}

constexpr void add_sensor_id(device& dev, sensor_id id) noexcept {
  std::visit([id](auto& d) { d.sensor_ids.push_back(id); }, dev);
}

} // namespace spymarine
