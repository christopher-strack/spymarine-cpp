#pragma once

#include "spymarine/byte_operations.hpp"

#include <algorithm>
#include <cstdint>
#include <span>
#include <string>
#include <variant>

namespace spymarine {

template <size_t StartIndex, size_t Size> class numeric_value {
public:
  constexpr explicit numeric_value(
      std::span<const uint8_t, Size> bytes) noexcept
      : _bytes{bytes} {}

  constexpr int16_t first() const noexcept {
    return to_int16(_bytes.template subspan<start_index(), 2>());
  }

  constexpr int16_t second() const noexcept {
    return to_int16(_bytes.template subspan<start_index() + 2, 2>());
  }

  constexpr int32_t number() const noexcept {
    return to_int32(_bytes.template subspan<start_index(), 4>());
  }

  constexpr static size_t start_index() noexcept { return StartIndex; }
  constexpr static size_t size() noexcept { return Size; }
  constexpr std::span<const uint8_t, Size> raw_bytes() const noexcept {
    return _bytes;
  }

private:
  std::span<const uint8_t, Size> _bytes;
};

using numeric_value1 = numeric_value<0, 7>;
using numeric_value3 = numeric_value<5, 12>;

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

struct invalid_value {
  bool operator<=>(const invalid_value& other) const noexcept = default;
};

using message_value =
    std::variant<numeric_value1, numeric_value3, string_value, invalid_value>;

} // namespace spymarine
