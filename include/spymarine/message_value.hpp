#pragma once

#include "spymarine/byte_operations.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <variant>

namespace spymarine {

using message_value_id = uint8_t;

class invalid_value {
public:
  constexpr invalid_value(message_value_id id) noexcept : _id{id} {}

  constexpr message_value_id id() const noexcept { return _id; }

  constexpr std::span<const uint8_t> raw_bytes() const noexcept { return {}; }

  bool operator<=>(const invalid_value& other) const noexcept = default;

private:
  message_value_id _id{0};
};

template <size_t StartIndex, size_t Size> class numeric_value {
public:
  constexpr explicit numeric_value(
      std::span<const uint8_t, Size> bytes) noexcept
      : _bytes{bytes} {}

  template <size_t S>
  static constexpr numeric_value<StartIndex, Size>
  from_bytes(std::span<const uint8_t, S> bytes) noexcept
    requires(S >= Size)
  {
    assert(bytes.size() >= Size); // for dynamic spans
    return numeric_value<StartIndex, Size>{bytes.template subspan<0, Size>()};
  }

  constexpr static size_t start_index() noexcept { return StartIndex; }

  constexpr static size_t raw_bytes_size() noexcept { return Size; }

  constexpr message_value_id id() const noexcept { return _bytes[0]; }

  constexpr int16_t low_int16() const noexcept {
    return to_int16(_bytes.template subspan<start_index(), 2>());
  }

  constexpr int16_t high_int16() const noexcept {
    return to_int16(_bytes.template subspan<start_index() + 2, 2>());
  }

  constexpr int32_t int32() const noexcept {
    return to_int32(_bytes.template subspan<start_index(), 4>());
  }

  constexpr std::span<const uint8_t, Size> raw_bytes() const noexcept {
    return _bytes;
  }

  constexpr bool
  operator==(const numeric_value<StartIndex, Size>& other) const noexcept {
    return std::ranges::equal(_bytes, other._bytes);
  }

private:
  std::span<const uint8_t, Size> _bytes;
};

using numeric_value1 = numeric_value<2, 6>;
using numeric_value3 = numeric_value<7, 11>;

constexpr uint8_t replace_non_ascii(uint8_t c) noexcept {
  return c < 128 ? c : '?';
}

class string_value {
public:
  constexpr explicit string_value(std::span<const uint8_t> bytes) noexcept
      : _bytes(bytes) {
    assert(bytes.size() >= 7);
  }

  static constexpr std::optional<string_value>
  from_bytes(std::span<const uint8_t> bytes) noexcept {
    const auto null_it =
        std::ranges::find(std::ranges::views::drop(bytes, 7), '\0');

    if (null_it != bytes.end()) {
      return string_value{std::span{bytes.begin(), null_it}};
    }

    return std::nullopt;
  }

  constexpr message_value_id id() const noexcept { return _bytes[0]; }

  constexpr auto raw_string_bytes() const noexcept {
    return _bytes.subspan(7) | std::ranges::views::transform(replace_non_ascii);
  }

  constexpr operator std::string() const noexcept {
    const auto bytes = raw_string_bytes();
    return std::string{bytes.begin(), bytes.end()};
  }

  constexpr std::string str() const noexcept { return std::string{*this}; }

  constexpr std::span<const uint8_t> raw_bytes() const noexcept {
    return _bytes;
  }

  constexpr bool operator==(const string_value& other) const noexcept {
    return std::ranges::equal(_bytes, other._bytes);
  }

  constexpr bool operator==(const std::string_view& other) const noexcept {
    return std::ranges::equal(raw_string_bytes(), other);
  }

private:
  std::span<const uint8_t> _bytes;
};

using message_value =
    std::variant<numeric_value1, numeric_value3, string_value, invalid_value>;

} // namespace spymarine
