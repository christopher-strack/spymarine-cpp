#pragma once

#include "device.hpp"

#include <ostream>

namespace spymarine {

std::ostream& operator<<(std::ostream& os, const sensor_type type);

std::ostream& operator<<(std::ostream& os, const sensor_info& info);

std::ostream& operator<<(std::ostream& os, const null_device& device);

std::ostream& operator<<(std::ostream& os, const pico_internal_device& device);

std::ostream& operator<<(std::ostream& os, const voltage_device& device);

std::ostream& operator<<(std::ostream& os, const current_device& device);

std::ostream& operator<<(std::ostream& os, const temperature_device& device);

std::ostream& operator<<(std::ostream& os, const barometer_device& device);

std::ostream& operator<<(std::ostream& os, const resistive_device& device);

std::ostream& operator<<(std::ostream& os, const fluid_type type);

std::ostream& operator<<(std::ostream& os, const tank_device& device);

std::ostream& operator<<(std::ostream& os, const battery_type type);

std::ostream& operator<<(std::ostream& os, const battery_device& device);

std::ostream& operator<<(std::ostream& os, const unknown_device& device);

std::ostream& operator<<(std::ostream& os, const device& device);

} // namespace spymarine
