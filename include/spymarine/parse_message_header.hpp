#pragma once

#include "spymarine/byte_operations.hpp"
#include "spymarine/error.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <expected>
#include <span>

namespace spymarine {

constexpr auto header_size = 14;

struct header {
  uint8_t type;
  uint16_t length;

  constexpr auto operator<=>(const header&) const noexcept = default;
};

constexpr std::expected<header, error>
parse_header(const std::span<const uint8_t> bytes) noexcept {
  if (bytes.size() < header_size) {
    return std::unexpected{parse_error::invalid_data_length};
  }

  const auto prefix =
      std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00, 0x00, 0xff});
  if (!std::ranges::equal(bytes.subspan(0, prefix.size()), prefix)) {
    return std::unexpected{parse_error::invalid_header};
  }

  if (bytes[13] != 0xff) {
    return std::unexpected{parse_error::invalid_header};
  }

  const auto type = bytes.data()[6];
  const auto length = to_uint16(bytes.subspan<11, 2>());

  return header{type, length};
}

} // namespace spymarine
