#pragma once

#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/overloaded.hpp"

#include <algorithm>
#include <cstdint>
#include <expected>
#include <variant>

namespace spymarine {
namespace detail {
template <typename T, typename... Args> struct variant_extender;

template <typename... Args0, typename... Args1>
struct variant_extender<std::variant<Args0...>, Args1...> {
  using type = std::variant<Args0..., Args1...>;
};
} // namespace detail

struct null_device {
  auto operator<=>(const null_device&) const = default;
};

struct unknown_device {
  auto operator<=>(const unknown_device&) const = default;
};

using parsed_device =
    detail::variant_extender<device, null_device, unknown_device>::type;

constexpr uint8_t sensor_state_offset(const parsed_device& d) noexcept {
  return std::visit(
      overloaded{
          [](const pico_internal_device&) -> uint8_t { return 6; },
          [](const voltage_device&) -> uint8_t { return 1; },
          [](const current_device&) -> uint8_t { return 2; },
          [](const temperature_device&) -> uint8_t { return 1; },
          [](const barometer_device&) -> uint8_t { return 2; },
          [](const resistive_device&) -> uint8_t { return 1; },
          [](const tank_device&) -> uint8_t { return 1; },
          [](const battery_device&) -> uint8_t { return 5; },
          [](const null_device&) -> uint8_t { return 0; },
          [](const unknown_device&) -> uint8_t { return 1; },
      },
      d);
}

constexpr fluid_type to_fluid_type(const int16_t type) noexcept {
  switch (type) {
  case 1:
    return fluid_type::fresh_water;
  case 2:
    return fluid_type::fuel;
  case 3:
    return fluid_type::waste_water;
  }
  return fluid_type::unknown;
}

constexpr battery_type to_battery_type(const int16_t battery_type) noexcept {
  switch (battery_type) {
  case 1:
    return battery_type::wet_low_maintenance;
  case 2:
    return battery_type::wet_maintenance_free;
  case 3:
    return battery_type::agm;
  case 4:
    return battery_type::deep_cycle;
  case 5:
    return battery_type::gel;
  case 6:
    return battery_type::lifepo4;
  }
  return battery_type::unknown;
}

constexpr std::expected<parsed_device, error>
parse_device(const message_values_view values,
             uint8_t state_start_index) noexcept {
  const auto type_value = values.find<numeric_value3>(1);
  if (!type_value) {
    return std::unexpected{parse_error::invalid_device_message};
  }

  const auto type = type_value->high_int16();
  auto name_value = values.find<string_value>(3);

  switch (type) {
  case 0:
    return null_device{};
  case 1:
    return name_value == "PICO INTERNAL"
               ? parsed_device{pico_internal_device{state_start_index}}
               : parsed_device{
                     voltage_device{name_value->str(), state_start_index}};
  case 2:
    if (name_value) {
      return current_device{name_value->str(), state_start_index};
    }
    break;
  case 3:
    if (name_value) {
      return temperature_device{name_value->str(), state_start_index};
    }
    break;
  case 4:
    return unknown_device{};
  case 5:
    if (name_value) {
      return barometer_device{name_value->str(), state_start_index};
    }
    break;
  case 6:
    if (name_value) {
      return resistive_device{name_value->str(), state_start_index};
    }
    break;
  case 7:
    return unknown_device{};
  case 8: {
    const auto fluid_type = values.find<numeric_value3>(6);
    const auto capacity = values.find<numeric_value3>(7);
    if (name_value && fluid_type && capacity) {
      return tank_device{name_value->str(),
          to_fluid_type(fluid_type->high_int16()),
          capacity->high_int16() / 10.0f,
          state_start_index,
      };
    }
    break;
  }
  case 9: {
    const auto battery_type = values.find<numeric_value3>(8);
    const auto capacity = values.find<numeric_value3>(5);
    if (name_value && battery_type && capacity) {
      return battery_device{name_value->str(),
          to_battery_type(battery_type->high_int16()),
          capacity->high_int16() / 100.0f,
          state_start_index,
      };
    }
    break;
  }
  case 10:
    return unknown_device{};
  case 14:
    return unknown_device{};
  default:
    return std::unexpected{parse_error::invalid_device_type};
  }

  return std::unexpected{parse_error::invalid_device_message};
}

} // namespace spymarine
