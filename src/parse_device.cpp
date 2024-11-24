#include "spymarine/parse_device.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/overloaded.hpp"

#include <algorithm>

namespace spymarine {

uint8_t sensor_state_offset(const parsed_device& device) {
  return std::visit(overloaded{
                        [](const pico_internal_device&) { return 6; },
                        [](const voltage_device&) { return 1; },
                        [](const current_device&) { return 2; },
                        [](const temperature_device&) { return 1; },
                        [](const barometer_device&) { return 2; },
                        [](const resistive_device&) { return 1; },
                        [](const tank_device&) { return 1; },
                        [](const battery_device&) { return 5; },
                        [](const null_device&) { return 0; },
                        [](const unknown_device&) { return 1; },
                    },
                    device);
}

namespace {
fluid_type to_fluid_type(const uint16_t type) {
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

battery_type to_battery_type(const uint16_t battery_type) {
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

std::string replace_non_ascii(const std::string_view& input) {
  std::string output;
  output.reserve(input.size());
  std::transform(
      input.begin(), input.end(), std::back_inserter(output),
      [](char c) { return static_cast<unsigned char>(c) < 128 ? c : '?'; });
  return output;
}
} // namespace

std::expected<parsed_device, error>
parse_device(const std::span<const uint8_t> bytes,
             const uint8_t state_start_index) {
  const auto values = message_values_view{bytes};
  const auto type_value = find_value_for_type<numeric_value>(1, values);
  if (!type_value) {
    return std::unexpected{parse_error::invalid_device_message};
  }

  const auto type = type_value->second();
  const auto name_value =
      find_value_for_type<std::string_view>(3, values).transform(
          replace_non_ascii);

  switch (type) {
  case 0:
    return null_device{};
  case 1:
    return name_value == "PICO INTERNAL"
               ? parsed_device{pico_internal_device{state_start_index}}
               : parsed_device{voltage_device{*name_value, state_start_index}};
  case 2:
    if (name_value) {
      return current_device{*name_value, state_start_index};
    }
    break;
  case 3:
    if (name_value) {
      return temperature_device{*name_value, state_start_index};
    }
    break;
  case 4:
    return unknown_device{};
  case 5:
    if (name_value) {
      return barometer_device{*name_value, state_start_index};
    }
    break;
  case 6:
    if (name_value) {
      return resistive_device{*name_value, state_start_index};
    }
    break;
  case 7:
    return unknown_device{};
  case 8: {
    const auto fluid_type = find_value_for_type<numeric_value>(6, bytes);
    const auto capacity = find_value_for_type<numeric_value>(7, bytes);
    if (name_value && fluid_type && capacity) {
      return tank_device{
          *name_value,
          to_fluid_type(fluid_type->second()),
          capacity->second() / 10.0f,
          state_start_index,
      };
    }
    break;
  }
  case 9: {
    const auto battery_type = find_value_for_type<numeric_value>(8, bytes);
    const auto capacity = find_value_for_type<numeric_value>(5, bytes);
    if (name_value && battery_type && capacity) {
      return battery_device{
          *name_value,
          to_battery_type(battery_type->second()),
          capacity->second() / 100.0f,
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
