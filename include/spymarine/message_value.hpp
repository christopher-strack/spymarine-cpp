#pragma once

#include "spymarine/byte_operations.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <variant>

namespace spymarine {

class numeric_value {
public:
  constexpr explicit numeric_value(std::span<const uint8_t, 4> bytes) noexcept {
    std::copy(bytes.begin(), bytes.end(), _bytes.begin());
  }

  constexpr int16_t first() const noexcept {
    return to_int16(std::span{_bytes}.subspan<0, 2>());
  }

  constexpr int16_t second() const noexcept {
    return to_int16(std::span{_bytes}.subspan<2, 2>());
  }

  constexpr int32_t number() const noexcept {
    return to_int32(std::span{_bytes});
  }

private:
  std::array<uint8_t, 4> _bytes;
};

class string_value {
public:
  constexpr explicit string_value(std::span<const uint8_t> bytes) noexcept
      : _bytes(bytes) {}

  constexpr operator std::string() const noexcept {
    return std::string{_bytes.begin(), _bytes.end()};
  }

  constexpr std::string str() const noexcept { return std::string{*this}; }

  constexpr size_t size() const noexcept { return _bytes.size(); }

private:
  std::span<const uint8_t> _bytes;
};

struct invalid_value {};

using message_value = std::variant<numeric_value, string_value, invalid_value>;

} // namespace spymarine
