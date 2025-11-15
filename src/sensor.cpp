#include "spymarine/sensor.hpp"

namespace spymarine {

float sensor_value(numeric_value1 value, sensor_type type) {
  switch (type) {
  case sensor_type::volume:
    return float(value.second()) / 10.0f;
  case sensor_type::level:
    return float(value.first()) / 10.0f;
  case sensor_type::voltage:
    return float(value.second()) / 1000.0f;
  case sensor_type::current:
    return float(value.second()) / 100.0f;
  case sensor_type::temperature:
    return float(value.second()) / 10.0f;
  case sensor_type::pressure:
    return float(value.number()) / 100.0f;
  case sensor_type::resistive:
    return float(value.second());
  case sensor_type::charge:
    return float(value.first()) / 160.0f;
  case sensor_type::capacity:
    return float(value.second()) / 100.0f;
  }

  return 0.0f;
}

} // namespace spymarine
