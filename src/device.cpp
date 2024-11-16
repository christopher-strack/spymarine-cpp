#include "spymarine/device.hpp"
#include "spymarine/overloaded.hpp"

namespace spymarine {

pico_internal_device::pico_internal_device(const uint8_t sensor_start_index)
    : voltage_sensor{sensor_type::voltage, sensor_start_index} {}

tank_device::tank_device(std::string name, fluid_type type, float capacity,
                         const uint8_t sensor_start_index)
    : name{std::move(name)}, type{type}, capacity{capacity},
      volume_sensor{sensor_type::volume, sensor_start_index},
      level_sensor{sensor_type::level, sensor_start_index} {}

voltage_device::voltage_device(std::string name,
                               const uint8_t sensor_start_index)
    : name{std::move(name)},
      voltage_sensor{sensor_type::voltage, sensor_start_index} {}

current_device::current_device(std::string name,
                               const uint8_t sensor_start_index)
    : name{std::move(name)},
      current_sensor{sensor_type::current, sensor_start_index} {}

temperature_device::temperature_device(std::string name,
                                       const uint8_t sensor_start_index)
    : name{std::move(name)},
      temperature_sensor{sensor_type::temperature, sensor_start_index} {}

barometer_device::barometer_device(std::string name,
                                   const uint8_t sensor_start_index)
    : name{std::move(name)},
      pressure_sensor{sensor_type::pressure, sensor_start_index} {}

resistive_device::resistive_device(std::string name,
                                   const uint8_t sensor_start_index)
    : name{std::move(name)},
      resistive_sensor{sensor_type::resistive, sensor_start_index} {}

battery_device::battery_device(std::string name, battery_type type,
                               float capacity, const uint8_t sensor_start_index)
    : name{std::move(name)}, type{type}, capacity{capacity},
      charge_sensor{sensor_type::charge, sensor_start_index},
      remaining_capacity_sensor{sensor_type::capacity, sensor_start_index},
      current_sensor{sensor_type::current, uint8_t(sensor_start_index + 1)},
      voltage_sensor{sensor_type::voltage, uint8_t(sensor_start_index + 2)} {}

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

} // namespace spymarine
