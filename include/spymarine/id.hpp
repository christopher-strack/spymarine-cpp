#pragma once

#include "spymarine/message_value.hpp"

namespace spymarine {

// sensor IDs can only be 8-bit values. Each sensor corresponds to a message
// value in the sensor state message.
using sensor_id = message_value_id;

// Each device has at least one sensor, so use the same type
using device_id = sensor_id;

} // namespace spymarine
