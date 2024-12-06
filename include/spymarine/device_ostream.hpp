#pragma once

#include "spymarine/device.hpp"

#include <iosfwd>

namespace spymarine {

std::ostream& operator<<(std::ostream& os, const sensor_type type);

std::ostream& operator<<(std::ostream& os, const sensor& info);

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

std::ostream& operator<<(std::ostream& os, const device& device);

} // namespace spymarine
