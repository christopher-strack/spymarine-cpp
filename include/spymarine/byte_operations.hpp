#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <span>

namespace spymarine {

constexpr uint16_t to_uint16(const std::span<const uint8_t, 2> data) noexcept {
  return std::bit_cast<uint16_t, std::array<uint8_t, 2>>({data[1], data[0]});
}

constexpr int16_t to_int16(const std::span<const uint8_t, 2> data) noexcept {
  return std::bit_cast<int16_t, std::array<uint8_t, 2>>({data[1], data[0]});
}

constexpr int32_t to_int32(const std::span<const uint8_t, 4> data) noexcept {
  return std::bit_cast<int32_t, std::array<uint8_t, 4>>(
      {data[3], data[2], data[1], data[0]});
}

} // namespace spymarine
