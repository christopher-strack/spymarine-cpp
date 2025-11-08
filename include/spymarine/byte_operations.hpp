#pragma once

#include <array>
#include <bit>
#include <span>

namespace spymarine {

constexpr uint16_t to_uint16(const std::span<const uint8_t, 2> data) noexcept {
  const auto native_order =
      std::bit_cast<uint16_t, std::array<uint8_t, 2>>({data[1], data[0]});

  if constexpr (std::endian::native == std::endian::big) {
    return std::byteswap(native_order);
  } else {
    return native_order;
  }
}

} // namespace spymarine
