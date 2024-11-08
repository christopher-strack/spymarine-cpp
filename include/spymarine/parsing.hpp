#pragma once

#include "device.hpp"

#include <optional>
#include <span>
#include <string_view>

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
std::optional<header> parse_header(const std::span<const uint8_t> data);

/* Calculate a CRC as accepted by Simarine devices.

  Original source: https://github.com/htool/pico2signalk
  Copyright Erik Bosman / @brainsmoke
*/
uint16_t crc(const std::span<const uint8_t> bytes);

/* Parse a response from a Simarine device. Returns a Message on success.
 * Raises ParsingError if the given data is not a valid or known Simarine
 * Message.
 */
std::optional<message>
parse_response(const std::span<const uint8_t> raw_response);

/* Returns a valid Simarine request for the given Message.
 */
std::span<uint8_t> make_request(message_type type,
                                const std::span<const uint8_t> data,
                                std::span<uint8_t> buffer);

/* Returns a device request for the given Device ID. The Simarine device
 * will reply with a device response that contains the description of the
 * device.
 */
std::span<uint8_t> make_device_request(uint8_t device_id,
                                       std::span<uint8_t> buffer);

/* Value in a PropertyDict that can either represent two independent 2 byte
 * numbers or a single 4 byte number
 */
class numeric_value {
public:
  explicit numeric_value(std::span<const uint8_t, 4> bytes);

  /* Returns the 2 first bytes as a number
   */
  uint16_t first() const;

  /* Returns the 2 last bytes as a number
   */
  uint16_t second() const;

  /* Returns all 4 bytes as a number
   */
  uint32_t number() const;

private:
  std::array<uint8_t, 4> _bytes;
};

using value = std::variant<numeric_value, std::string_view>;

std::optional<device> parse_device(const std::span<const uint8_t> bytes);

} // namespace spymarine
