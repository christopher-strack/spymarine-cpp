#pragma once

#include <array>
#include <bit>
#include <span>

namespace spymarine {

constexpr uint16_t to_uint16(const std::span<const uint8_t, 2> data) noexcept {
  return std::bit_cast<uint16_t, std::array<uint8_t, 2>>({data[1], data[0]});
}

} // namespace spymarine
