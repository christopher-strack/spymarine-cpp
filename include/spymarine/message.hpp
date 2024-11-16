#pragma once

#include <cstdint>
#include <span>

namespace spymarine {

/* The message type described by the header.
 * enum only represents the known subset.
 */
enum class message_type {
  // Request the number of connected devices
  device_count = 0x02,

  // Request information about a device
  device_info = 0x41,

  // Sensor update message (UDP)
  sensor_state = 0xb0,
};

/* A message as received by a Simarine device
 */
struct message {
  message_type type;
  std::span<const uint8_t> data;
};

bool operator==(const message& lhs, const message& rhs);
bool operator!=(const message& lhs, const message& rhs);

} // namespace spymarine
