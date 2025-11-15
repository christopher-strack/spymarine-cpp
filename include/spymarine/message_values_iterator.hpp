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

struct id_and_value {
  message_value_id id;
  message_value value;
};

constexpr std::optional<string_value>
read_string_value(std::span<const uint8_t> bytes) noexcept {
  if (bytes.size() >= 5) {
    const auto data = bytes.subspan(5);
    const auto it = std::ranges::find(data, 0);
    if (it != data.end()) {
      const auto size = static_cast<size_t>(std::distance(data.begin(), it));
      return string_value{data.subspan(0, size)};
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
  using value_type = id_and_value;
  using difference_type = std::ptrdiff_t;

public:
  constexpr message_values_iterator() noexcept : _data{0, invalid_value{}} {}

  constexpr explicit message_values_iterator(
      std::span<const uint8_t> buffer) noexcept
      : _data{0, invalid_value{}}, _bytes{buffer} {
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

    std::visit(overloaded{[this](const numeric_value1&) {
                            _bytes = advance_bytes(_bytes, 7);
                          },
                          [this](const numeric_value3&) {
                            _bytes = advance_bytes(_bytes, 12);
                          },
                          [this](const string_value& sv) {
                            _bytes = advance_bytes(_bytes, sv.size() + 9);
                          },
                          [this](const invalid_value&) { _bytes = {}; }},
               _data.value);

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

    _data.id = _bytes[0];

    const auto type = _bytes[1];
    const auto payload = _bytes.subspan(2);

    if (type == 1 && _bytes.size() >= 6) {
      _data.value = numeric_value1{_bytes.subspan<0, 6>()};
    } else if (type == 3 && _bytes.size() >= 11) {
      _data.value = numeric_value3{_bytes.subspan<0, 11>()};
    } else if (type == 4) {
      const auto sv = read_string_value(payload);
      _data.value = sv ? message_value{*sv} : invalid_value{};
    } else {
      _data.value = invalid_value{};
    }
  }

  id_and_value _data;
  std::span<const uint8_t> _bytes;
};

} // namespace spymarine
