#pragma once

#include "spymarine/device.hpp"
#include "spymarine/message.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/overloaded.hpp"

#include <expected>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace spymarine {

using sensor_map = std::unordered_map<uint8_t, std::vector<sensor*>>;

sensor_map build_sensor_map(std::vector<device>& devices_range);

void update_sensor_states(message state_message, sensor_map& map);

} // namespace spymarine
