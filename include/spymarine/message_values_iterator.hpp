#pragma once

#include "spymarine/message_value.hpp"
#include "spymarine/overloaded.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <optional>
#include <span>
#include <type_traits>

namespace spymarine {

constexpr std::optional<string_value>
read_string_value(std::span<const uint8_t> bytes) noexcept {
  if (bytes.size() >= 7) {
    const auto data = bytes.subspan(7);
    const auto it = std::ranges::find(data, 0);
    if (it != data.end()) {
      const auto size = static_cast<size_t>(std::distance(data.begin(), it));
      return string_value{bytes.subspan(0, 7 + size)};
    }
  }
  return std::nullopt;
}

constexpr std::span<const uint8_t> advance_bytes(std::span<const uint8_t> bytes,
                                                 size_t n) noexcept {
  return bytes.size() >= n + 2 ? bytes.subspan(n) : std::span<const uint8_t>{};
}

class message_values_iterator {
public:
  using iterator_category = std::input_iterator_tag;
  using value_type = message_value;
  using difference_type = std::ptrdiff_t;

public:
  constexpr message_values_iterator() noexcept : _data{invalid_value{0}} {}

  constexpr explicit message_values_iterator(
      std::span<const uint8_t> buffer) noexcept
      : _data{invalid_value{0}}, _bytes{buffer} {
    update_data();
  }

  constexpr value_type operator*() const noexcept { return _data; }

  constexpr std::add_pointer_t<value_type const> operator->() const noexcept {
    return std::addressof(_data);
  }

  constexpr message_values_iterator& operator++() noexcept {
    if (_bytes.size() < 2) {
      _bytes = {};
      return *this;
    }

    _bytes = std::visit(
        overloaded{
            [this](const numeric_value1&) {
              return advance_bytes(_bytes, numeric_value1::size() + 1);
            },
            [this](const numeric_value3&) {
              return advance_bytes(_bytes, numeric_value3::size() + 1);
            },
            [this](const string_value& sv) {
              return advance_bytes(_bytes, sv.size() + 9);
            },
            [](const invalid_value&) { return std::span<const uint8_t>{}; }},
        _data);

    update_data();

    return *this;
  }

  constexpr message_values_iterator operator++(int) noexcept {
    message_values_iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr friend bool
  operator==(const message_values_iterator& lhs,
             const message_values_iterator& rhs) noexcept {
    return std::ranges::equal(lhs._bytes, rhs._bytes);
  }

  constexpr friend bool
  operator!=(const message_values_iterator& lhs,
             const message_values_iterator& rhs) noexcept {
    return !(lhs == rhs);
  }

private:
  constexpr void update_data() noexcept {
    if (_bytes.size() < 2) {
      return;
    }

    const auto type = _bytes[1];

    if (type == 1 && _bytes.size() >= numeric_value1::size()) {
      _data = numeric_value1::from_bytes(_bytes);
    } else if (type == 3 && _bytes.size() >= numeric_value3::size()) {
      _data = numeric_value3::from_bytes(_bytes);
    } else if (type == 4) {
      const auto sv = read_string_value(_bytes);
      _data = sv ? message_value{*sv} : invalid_value{_bytes[0]};
    } else {
      _data = invalid_value{_bytes[0]};
    }
  }

  message_value _data;
  std::span<const uint8_t> _bytes;
};

} // namespace spymarine
