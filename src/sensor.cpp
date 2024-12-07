#include "spymarine/sensor.hpp"

namespace spymarine {

float sensor_value(numeric_value value, sensor_type type) {
  switch (type) {
  case sensor_type::volume:
    return value.second() / 10.0;
  case sensor_type::level:
    return value.first() / 10.0;
  case sensor_type::voltage:
    return value.second() / 1000.0f;
  case sensor_type::current:
    return value.second() / 100.0f;
  case sensor_type::temperature:
    return value.second() / 10.0f;
  case sensor_type::pressure:
    return value.number() / 100.0f;
  case sensor_type::resistive:
    return value.second();
  case sensor_type::charge:
    return value.first() / 160.0;
  case sensor_type::capacity:
    return value.second() / 100.0;
  }

  return 0.0f;
}

} // namespace spymarine
