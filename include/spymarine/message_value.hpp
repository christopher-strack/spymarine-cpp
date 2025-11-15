#pragma once

#include "spymarine/byte_operations.hpp"

#include <algorithm>
#include <cstdint>
#include <span>
#include <string>
#include <variant>

namespace spymarine {

using message_value_id = uint8_t;

template <size_t StartIndex, size_t Size> class numeric_value {
public:
  constexpr explicit numeric_value(
      std::span<const uint8_t, Size> bytes) noexcept
      : _bytes{bytes} {}

  constexpr message_value_id id() const noexcept { return _bytes[0]; }

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

using numeric_value1 = numeric_value<2, 6>;
using numeric_value3 = numeric_value<7, 11>;

class string_value {
public:
  constexpr explicit string_value(std::span<const uint8_t> bytes) noexcept
      : _bytes(bytes) {}

  constexpr message_value_id id() const noexcept { return _bytes[0]; }

  constexpr operator std::string() const noexcept {
    const auto data = _bytes.subspan(7);
    return std::string{data.begin(), data.end()};
  }

  constexpr std::string str() const noexcept { return std::string{*this}; }

  constexpr size_t size() const noexcept { return _bytes.size() - 7; }

private:
  std::span<const uint8_t> _bytes;
};

struct invalid_value {
  bool operator<=>(const invalid_value& other) const noexcept = default;
};

using message_value =
    std::variant<numeric_value1, numeric_value3, string_value, invalid_value>;

} // namespace spymarine
