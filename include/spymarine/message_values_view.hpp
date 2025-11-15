#pragma once

#include "spymarine/message_values_iterator.hpp"

#include <cstdint>
#include <optional>
#include <span>

namespace spymarine {

class message_values_view
    : public std::ranges::view_interface<message_values_view> {
public:
  message_values_view() = default;
  constexpr message_values_view(std::span<const uint8_t> buffer) noexcept
      : _buffer{buffer} {}

  constexpr message_values_iterator begin() const noexcept {
    return message_values_iterator(_buffer);
  }

  constexpr message_values_iterator end() const noexcept {
    return message_values_iterator(std::span<const uint8_t>{});
  }

private:
  std::span<const uint8_t> _buffer;
};

constexpr message_values_iterator
find_value(const message_value_id id,
           const message_values_view& values) noexcept {
  return std::ranges::find_if(values, [id](const auto value) {
    return get_message_value_id(value) == id;
  });
}

template <typename T>
constexpr std::optional<T>
find_value_for_type(const message_value_id id,
                    const message_values_view& values) noexcept {
  if (const auto it = find_value(id, values); it != values.end()) {
    const auto& v = *it;
    if (const auto* value = std::get_if<T>(&v); value) {
      return *value;
    }
  }
  return std::nullopt;
}

} // namespace spymarine
