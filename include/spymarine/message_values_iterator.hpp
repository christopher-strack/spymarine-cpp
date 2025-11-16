#pragma once

#include "spymarine/message_value.hpp"
#include "spymarine/overloaded.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <span>
#include <type_traits>

namespace spymarine {

class message_values_iterator {
public:
  using iterator_category = std::input_iterator_tag;
  using value_type = message_value;
  using difference_type = std::ptrdiff_t;
  using pointer = const value_type*;
  using reference = const value_type&;

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
    _bytes = std::visit(overloaded{[this](const numeric_value1&) {
                                     return std::ranges::views::drop(
                                         _bytes, numeric_value1::size() + 1);
                                   },
                                   [this](const numeric_value3&) {
                                     return std::ranges::views::drop(
                                         _bytes, numeric_value3::size() + 1);
                                   },
                                   [this](const string_value& sv) {
                                     // null terminator + delimiter
                                     return std::ranges::views::drop(
                                         _bytes, sv.raw_bytes().size() + 2);
                                   },
                                   [](const invalid_value&) {
                                     return std::span<const uint8_t>{};
                                   }},
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

private:
  constexpr void update_data() noexcept {
    if (_bytes.size() < 2) {
      _bytes = {};
      _data = invalid_value{0};
      return;
    }

    const auto type = _bytes[1];

    if (type == 1 && _bytes.size() >= numeric_value1::size()) {
      _data = numeric_value1::from_bytes(_bytes);
    } else if (type == 3 && _bytes.size() >= numeric_value3::size()) {
      _data = numeric_value3::from_bytes(_bytes);
    } else if (type == 4) {
      if (const auto sv = string_value::from_bytes(_bytes)) {
        _data = *sv;
      } else {
        _data = invalid_value{_bytes[0]};
      }
    } else {
      _data = invalid_value{_bytes[0]};
    }
  }

  message_value _data;
  std::span<const uint8_t> _bytes;
};

static_assert(std::input_iterator<message_values_iterator>);

} // namespace spymarine
