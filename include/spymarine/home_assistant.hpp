#pragma once

#include "spymarine/device.hpp"

#include <string>

namespace spymarine {

struct mqtt_message {
  std::string topic;
  std::string payload;
};

mqtt_message make_home_assistant_device_discovery_message(const device& device);

mqtt_message make_home_assistant_state_message(const device& device);

} // namespace spymarine
