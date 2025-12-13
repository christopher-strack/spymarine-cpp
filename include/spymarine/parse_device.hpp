#pragma once

#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"

#include <cstdint>
#include <expected>

namespace spymarine {

constexpr fluid_type to_fluid_type(const numeric_value3 type) noexcept {
  switch (type.uint32()) {
  case 1:
    return fluid_type::fresh_water;
  case 2:
    return fluid_type::fuel;
  case 3:
    return fluid_type::waste_water;
  }
  return fluid_type::unknown;
}

constexpr battery_type to_battery_type(const numeric_value3 type) noexcept {
  switch (type.uint32()) {
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

constexpr std::expected<device, error>
parse_device2(const message_values_view values) noexcept {
  const auto id_value = values.find<numeric_value1>(0);
  const auto type_value = values.find<numeric_value3>(1);

  if (!id_value || !type_value) {
    return std::unexpected{parse_error::invalid_device_message};
  }

  const auto id = id_value->uint8();
  const auto type = type_value->uint32();
  auto name = values.find<string_value>(3).transform(
      [](const auto& v) { return v.str(); });

  switch (type) {
  case 1:
    return device{voltage_device{.id = id, .name = std::move(name)}};
  case 2:
    return current_device{.id = id, .name = std::move(name)};
  case 3:
    return temperature_device{.id = id, .name = std::move(name)};
  case 5:
    return barometer_device{.id = id, .name = std::move(name)};
  case 6:
    return resistive_device{.id = id, .name = std::move(name)};
  case 8:
    return tank_device{
        .id = id,
        .name = std::move(name),
        .type = values.find<numeric_value3>(6).transform(to_fluid_type),
        .capacity = values.find<numeric_value3>(7).transform(
            [](const auto& v) { return tank_capacity{v.high_int16()}; }),
    };
  case 9:
    return battery_device{
        .id = id,
        .name = std::move(name),
        .type = values.find<numeric_value3>(8).transform(to_battery_type),
        .capacity = values.find<numeric_value3>(5).transform(
            [](const auto& v) { return battery_capacity{v.high_int16()}; }),
    };
  }

  return unsupported_device{
      .id = id,
      .raw_type = static_cast<uint32_t>(type),
      .name = std::move(name),
  };
}

} // namespace spymarine
