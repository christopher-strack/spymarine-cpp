#pragma once

#include "spymarine/device.hpp"
#include "spymarine/parse_device.hpp"

#include <iosfwd>

namespace spymarine {

std::ostream& operator<<(std::ostream& os, const sensor_type type);

std::ostream& operator<<(std::ostream& os, const sensor& info);

std::ostream& operator<<(std::ostream& os, const pico_internal_device& d);

std::ostream& operator<<(std::ostream& os, const voltage_device& d);

std::ostream& operator<<(std::ostream& os, const current_device& d);

std::ostream& operator<<(std::ostream& os, const temperature_device& d);

std::ostream& operator<<(std::ostream& os, const barometer_device& d);

std::ostream& operator<<(std::ostream& os, const resistive_device& d);

std::ostream& operator<<(std::ostream& os, const fluid_type type);

std::ostream& operator<<(std::ostream& os, const tank_device& d);

std::ostream& operator<<(std::ostream& os, const battery_type type);

std::ostream& operator<<(std::ostream& os, const battery_device& d);

std::ostream& operator<<(std::ostream& os, const device& d);

std::ostream& operator<<(std::ostream& os, const null_device& d);

std::ostream& operator<<(std::ostream& os, const unknown_device& d);

std::ostream& operator<<(std::ostream& os, const parsed_device& d);

} // namespace spymarine
