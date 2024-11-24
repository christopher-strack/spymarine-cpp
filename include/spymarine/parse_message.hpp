#pragma once

#include "spymarine/error.hpp"
#include "spymarine/message.hpp"

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
