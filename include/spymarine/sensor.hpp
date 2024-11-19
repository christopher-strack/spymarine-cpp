#pragma once

#include "spymarine/message_value.hpp"

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

struct sensor {
  sensor_type type;
  uint8_t state_index;
  float value;

  auto operator<=>(const sensor&) const = default;
};

float sensor_value(numeric_value value, sensor_type type);

} // namespace spymarine
