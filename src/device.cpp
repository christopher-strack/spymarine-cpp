#include "spymarine/device.hpp"

namespace spymarine {

pico_internal_device::pico_internal_device(const uint8_t state_start_index)
    : device_sensor{sensor_type::voltage, state_start_index} {}

tank_device::tank_device(std::string name, fluid_type type, float capacity,
                         const uint8_t state_start_index)
    : name{std::move(name)}, type{type}, capacity{capacity},
      volume_sensor{sensor_type::volume, state_start_index},
      level_sensor{sensor_type::level, state_start_index} {}

voltage_device::voltage_device(std::string name,
                               const uint8_t state_start_index)
    : name{std::move(name)},
      device_sensor{sensor_type::voltage, state_start_index} {}

current_device::current_device(std::string name,
                               const uint8_t state_start_index)
    : name{std::move(name)},
      device_sensor{sensor_type::current, state_start_index} {}

temperature_device::temperature_device(std::string name,
                                       const uint8_t state_start_index)
    : name{std::move(name)},
      device_sensor{sensor_type::temperature, state_start_index} {}

barometer_device::barometer_device(std::string name,
                                   const uint8_t state_start_index)
    : name{std::move(name)},
      device_sensor{sensor_type::pressure, state_start_index} {}

resistive_device::resistive_device(std::string name,
                                   const uint8_t state_start_index)
    : name{std::move(name)},
      device_sensor{sensor_type::resistive, state_start_index} {}

battery_device::battery_device(std::string name, battery_type type,
                               float capacity, const uint8_t state_start_index)
    : name{std::move(name)}, type{type}, capacity{capacity},
      charge_sensor{sensor_type::charge, state_start_index},
      remaining_capacity_sensor{sensor_type::capacity, state_start_index},
      current_sensor{sensor_type::current, uint8_t(state_start_index + 1)},
      voltage_sensor{sensor_type::voltage, uint8_t(state_start_index + 2)} {}

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

void update_sensor_states(message state_message, sensor_map& map) {
  if (state_message.type != message_type::sensor_state) {
    return;
  }

  message_values_view state_values{state_message.data};

  for (const auto& entry : state_values) {
    if (const auto value = std::get_if<numeric_value>(&entry.value)) {
      if (const auto it = map.find(entry.id); it != map.end()) {
        for (sensor* sensor : it->second) {
          sensor->value = sensor_value(*value, sensor->type);
        }
      }
    }
  }
}

} // namespace spymarine
