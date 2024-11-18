#include "spymarine/device.hpp"

namespace spymarine {

pico_internal_device::pico_internal_device(const uint8_t state_start_index,
                                           const float sensor_value)
    : device_sensor{sensor_type::voltage, state_start_index, sensor_value} {}

tank_device::tank_device(std::string name, fluid_type type, float capacity,
                         const uint8_t state_start_index, const float volume,
                         const float level)
    : name{std::move(name)}, type{type}, capacity{capacity},
      volume_sensor{sensor_type::volume, state_start_index, volume},
      level_sensor{sensor_type::level, state_start_index, level} {}

voltage_device::voltage_device(std::string name,
                               const uint8_t state_start_index,
                               const float sensor_value)
    : name{std::move(name)},
      device_sensor{sensor_type::voltage, state_start_index, sensor_value} {}

current_device::current_device(std::string name,
                               const uint8_t state_start_index,
                               const float sensor_value)
    : name{std::move(name)},
      device_sensor{sensor_type::current, state_start_index, sensor_value} {}

temperature_device::temperature_device(std::string name,
                                       const uint8_t state_start_index,
                                       const float sensor_value)
    : name{std::move(name)},
      device_sensor{sensor_type::temperature, state_start_index, sensor_value} {
}

barometer_device::barometer_device(std::string name,
                                   const uint8_t state_start_index,
                                   const float sensor_value)
    : name{std::move(name)},
      device_sensor{sensor_type::pressure, state_start_index, sensor_value} {}

resistive_device::resistive_device(std::string name,
                                   const uint8_t state_start_index,
                                   const float sensor_value)
    : name{std::move(name)},
      device_sensor{sensor_type::resistive, state_start_index, sensor_value} {}

battery_device::battery_device(std::string name, const battery_type type,
                               const float capacity,
                               const uint8_t state_start_index,
                               const float charge,
                               const float remaining_capacity,
                               const float current, const float voltage)
    : name{std::move(name)}, type{type}, capacity{capacity},
      charge_sensor{sensor_type::charge, state_start_index, charge},
      remaining_capacity_sensor{sensor_type::capacity, state_start_index,
                                remaining_capacity},
      current_sensor{sensor_type::current, uint8_t(state_start_index + 1),
                     current},
      voltage_sensor{sensor_type::voltage, uint8_t(state_start_index + 2),
                     voltage} {}

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
    return value.number();
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
