#pragma once

#include "spymarine/device.hpp"

#include <optional>
#include <string>

namespace spymarine {

std::optional<std::string>
make_home_assistant_device_discovery_message(const device& device);

std::optional<std::string>
make_home_assistant_sensor_message(const device& device);

} // namespace spymarine
