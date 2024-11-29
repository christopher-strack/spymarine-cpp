#pragma once

#include "spymarine/device.hpp"

#include <cstdint>
#include <optional>
#include <string>

namespace spymarine {

struct mqtt_message {
  std::string topic;
  std::string payload;
};

mqtt_message make_home_assistant_device_discovery(const device& device);

std::optional<std::string>
make_home_assistant_sensor_topic(const device& device);

std::optional<std::string>
make_home_assistant_sensor_message(const device& device);

} // namespace spymarine
