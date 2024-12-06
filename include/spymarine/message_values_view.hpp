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
  message_values_view(std::span<const uint8_t> buffer);

  message_values_iterator begin() const;
  message_values_iterator end() const;

private:
  std::span<const uint8_t> _buffer;
};

message_values_iterator find_value(const uint8_t id,
                                   const message_values_view& values);

template <typename T>
std::optional<T> find_value_for_type(const uint8_t id,
                                     const message_values_view& values) {
  if (const auto it = find_value(id, values); it != values.end()) {
    if (const auto* value = std::get_if<T>(&it->value); value) {
      return *value;
    }
  }
  return std::nullopt;
}

} // namespace spymarine
