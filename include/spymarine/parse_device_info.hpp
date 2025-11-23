#pragma once

#include "spymarine/device_info.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"

#include <cstdint>
#include <expected>

namespace spymarine {

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

constexpr std::expected<device_info, error>
parse_device_info(const message_values_view values) noexcept {
  const auto id_value = values.find<numeric_value1>(0);
  if (!id_value) {
    return std::unexpected{parse_error::invalid_device_message};
  }

  const auto type_value = values.find<numeric_value3>(1);
  if (!type_value) {
    return std::unexpected{parse_error::invalid_device_message};
  }

  const auto id = id_value->uint32();
  const auto type = type_value->int32();
  auto name_value = values.find<string_value>(3);

  switch (type) {
  case 0:
    return null_device_info{.id = id};
  case 1:
    return name_value == "PICO INTERNAL"
               ? device_info{pico_internal_device_info{.id = id}}
               : device_info{
                     voltage_device_info{.id = id, .name = name_value->str()}};
  case 2:
    if (name_value) {
      return current_device_info{.id = id, .name = name_value->str()};
    }
    break;
  case 3:
    if (name_value) {
      return temperature_device_info{.id = id, .name = name_value->str()};
    }
    break;
  case 4:
    return unknown_device_info{.id = id};
  case 5:
    if (name_value) {
      return barometer_device_info{.id = id, .name = name_value->str()};
    }
    break;
  case 6:
    if (name_value) {
      return resistive_device_info{.id = id, .name = name_value->str()};
    }
    break;
  case 7:
    return unknown_device_info{.id = id};
  case 8: {
    const auto fluid_type = values.find<numeric_value3>(6);
    const auto capacity = values.find<numeric_value3>(7);
    if (name_value && fluid_type && capacity) {
      return tank_device_info{
          .id = id,
          .name = name_value->str(),
          .type = to_fluid_type(fluid_type->high_int16()),
          .capacity = tank_capacity{capacity->high_int16()},
      };
    }
    break;
  }
  case 9: {
    const auto battery_type = values.find<numeric_value3>(8);
    const auto capacity = values.find<numeric_value3>(5);
    if (name_value && battery_type && capacity) {
      return battery_device_info{
          .id = id,
          .name = name_value->str(),
          .type = to_battery_type(battery_type->high_int16()),
          .capacity = battery_capacity{capacity->high_int16()},
      };
    }
    break;
  }
  case 10:
    return unknown_device_info{.id = id};
  case 14:
    return unknown_device_info{.id = id};
  default:
    return std::unexpected{parse_error::invalid_device_type};
  }

  return std::unexpected{parse_error::invalid_device_message};
}

} // namespace spymarine
