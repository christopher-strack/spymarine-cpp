#pragma once

#include "spymarine/value_iterator.hpp"

#include <cstdint>
#include <optional>
#include <span>
#include <variant>

namespace spymarine {

class value_view : public std::ranges::view_interface<value_view> {
public:
  value_view() = default;
  value_view(std::span<const uint8_t> buffer);

  value_iterator begin() const;
  value_iterator end() const;

private:
  std::span<const uint8_t> _buffer;
};

value_iterator find_value(const uint8_t id, const value_view& values);

template <typename T>
std::optional<T> find_value_for_type(const uint8_t id,
                                     const value_view& values) {
  if (const auto it = find_value(id, values); it != values.end()) {
    if (const auto* value = std::get_if<T>(&it->value); value) {
      return *value;
    }
  }
  return std::nullopt;
}

} // namespace spymarine
