#pragma once

#include "spymarine/value.hpp"

#include <cstdint>
#include <iterator>
#include <span>
#include <type_traits>

namespace spymarine {

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

} // namespace spymarine
