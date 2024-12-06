#pragma once

#include "spymarine/error.hpp"
#include "spymarine/message.hpp"

#include <expected>
#include <span>

namespace spymarine {

constexpr auto header_size = 14;

struct header {
  uint8_t type;
  uint16_t length;

  auto operator<=>(const header&) const = default;
};

std::expected<header, error> parse_header(const std::span<const uint8_t> bytes);

/* Original source: https://github.com/htool/pico2signalk
 * Copyright Erik Bosman / @brainsmoke
 */
uint16_t crc(const std::span<const uint8_t> bytes);

std::expected<message, error>
parse_message(const std::span<const uint8_t> bytes);

} // namespace spymarine
