#pragma once

#include "spymarine/message_values_iterator.hpp"

#include <cstdint>
#include <optional>
#include <span>

namespace spymarine {

class message_values_view
    : public std::ranges::view_interface<message_values_view> {
public:
  constexpr message_values_view() noexcept = default;
  constexpr message_values_view(std::span<const uint8_t> buffer) noexcept
      : _buffer{buffer} {}

  constexpr message_values_iterator begin() const noexcept {
    return message_values_iterator{_buffer};
  }

  constexpr message_values_iterator end() const noexcept {
    return message_values_iterator{};
  }

  constexpr message_values_iterator
  find(const message_value_id id) const noexcept {
    return std::ranges::find_if(*this, [id](const auto value) {
      return get_message_value_id(value) == id;
    });
  }

  template <typename T>
  constexpr std::optional<T> find(const message_value_id id) const noexcept {
    if (const auto it = find(id); it != end()) {
      const auto& v = *it;
      if (const auto* value = std::get_if<T>(&v); value) {
        return *value;
      }
    }
    return std::nullopt;
  }

private:
  std::span<const uint8_t> _buffer;
};

} // namespace spymarine
