#pragma once

#include "spymarine/error.hpp"

#include <expected>
#include <span>

namespace spymarine {

constexpr auto header_size = 14;

/* Describes the header of a message
 */
struct header {
  uint8_t type;
  uint16_t length;

  auto operator<=>(const header&) const = default;
};

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

/* Parses the given bytes. Returns a Header on success.
    Raises ParsingError if the given bytes are not a valid header.
    Note: The header is not fully understood and might not work on
    all Simarine devices.
 */
std::expected<header, error> parse_header(const std::span<const uint8_t> data);

/* Calculate a CRC as accepted by Simarine devices.

  Original source: https://github.com/htool/pico2signalk
  Copyright Erik Bosman / @brainsmoke
*/
uint16_t crc(const std::span<const uint8_t> bytes);

/* Parse a response from a Simarine device. Returns a Message on success.
 * Raises ParsingError if the given data is not a valid or known Simarine
 * Message.
 */
std::expected<message, error>
parse_message(const std::span<const uint8_t> data);

} // namespace spymarine
