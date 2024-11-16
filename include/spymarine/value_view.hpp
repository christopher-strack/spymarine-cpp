#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string_view>
#include <type_traits>
#include <variant>

namespace spymarine {

class numeric_value {
public:
  explicit numeric_value(std::span<const uint8_t, 4> bytes);

  uint16_t first() const;
  uint16_t second() const;
  uint32_t number() const;

private:
  std::array<uint8_t, 4> _bytes;
};

struct invalid_value {};

using value = std::variant<numeric_value, std::string_view, invalid_value>;

struct id_and_value {
  uint8_t id;
  value value;
};

class value_iterator {
public:
  using iterator_category = std::input_iterator_tag;
  using value_type = id_and_value;
  using difference_type = std::ptrdiff_t;

public:
  value_iterator();
  explicit value_iterator(std::span<const uint8_t> buffer);

  value_type operator*() const;

  std::add_pointer_t<value_type const> operator->() const;

  value_iterator& operator++();

  value_iterator operator++(int);

  friend bool operator==(const value_iterator& lhs, const value_iterator& rhs);

  friend bool operator!=(const value_iterator& lhs, const value_iterator& rhs);

private:
  void update_id_and_value();

  id_and_value _id_and_value;
  std::span<const uint8_t> _remaining;
};

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
