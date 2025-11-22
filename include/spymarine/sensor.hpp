#pragma once

#include "spymarine/message_value.hpp"

#include <cstdint>

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

constexpr float sensor_value(numeric_value1 value, sensor_type type) noexcept {
  switch (type) {
  case sensor_type::volume:
    return float(value.high_int16()) / 10.0f;
  case sensor_type::level:
    return float(value.low_int16()) / 10.0f;
  case sensor_type::voltage:
    return float(value.high_int16()) / 1000.0f;
  case sensor_type::current:
    return float(value.high_int16()) / 100.0f;
  case sensor_type::temperature:
    return float(value.high_int16()) / 10.0f;
  case sensor_type::pressure:
    return float(value.int32()) / 100.0f;
  case sensor_type::resistive:
    return float(value.high_int16());
  case sensor_type::charge:
    return float(value.low_int16()) / 160.0f;
  case sensor_type::capacity:
    return float(value.high_int16()) / 100.0f;
  }

  return 0.0f;
}

} // namespace spymarine
