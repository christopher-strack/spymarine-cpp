#pragma once

#include "spymarine/error.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/sensor.hpp"

#include <expected>

namespace spymarine {

constexpr std::expected<sensor, error>
parse_sensor2(const message_values_view values) noexcept {
  const auto id_value = values.find<numeric_value1>(1);
  const auto type_value = values.find<numeric_value1>(2);

  if (!id_value || !type_value) {
    return std::unexpected{parse_error::invalid_sensor_message};
  }

  const auto id = id_value->uint8();
  const auto device_local_id = values.find<numeric_value1>(4).transform(
      [](auto n) { return n.uint8(); });
  const auto parent_device_id = values.find<numeric_value1>(3).transform(
      [](auto n) { return n.uint32(); });

  switch (type_value->uint8()) {
  case 1:
    return voltage_sensor{.id = id, .parent_device_id = parent_device_id};
  case 2:
    return current_sensor{.id = id, .parent_device_id = parent_device_id};
  case 4:
    return temperature_sensor{.id = id, .parent_device_id = parent_device_id};
  case 5:
    return device_local_id == 0
               ? sensor{barometer_trend_sensor{
                     .id = id, .parent_device_id = parent_device_id}}
               : sensor{barometer_sensor{.id = id,
                                         .parent_device_id = parent_device_id}};
  case 7:
    return resistive_sensor{.id = id, .parent_device_id = parent_device_id};
  case 8:
    return tank_sensor{.id = id, .parent_device_id = parent_device_id};
  case 11:
    return battery_sensor{.id = id, .parent_device_id = parent_device_id};
  };

  return unsupported_sensor{
      .id = id,
      .parent_device_id = parent_device_id,
      .raw_type = type_value->uint8(),
  };
}

} // namespace spymarine
