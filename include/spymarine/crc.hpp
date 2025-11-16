#pragma once

#include <cstdint>
#include <span>

namespace spymarine {

/* Original source: https://github.com/htool/pico2signalk
 * Copyright Erik Bosman / @brainsmoke
 */
constexpr uint16_t crc(const std::span<const uint8_t> bytes) noexcept {
  const uint16_t poly = 0x1189;
  uint16_t crc = 0;

  for (auto byte : bytes) {
    crc ^= uint16_t(byte << 8);
    for (int i = 0; i < 8; i++) {
      if (crc & 0x8000) {
        crc = uint16_t(crc << 1) ^ poly;
      } else {
        crc = uint16_t(crc << 1);
      }
    }
  }

  return crc;
}

} // namespace spymarine
