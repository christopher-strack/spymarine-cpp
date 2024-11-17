#pragma once

#include "spymarine/message_value.hpp"

#include <cstdint>
#include <iterator>
#include <span>
#include <type_traits>

namespace spymarine {

struct id_and_value {
  uint8_t id;
  message_value value;
};

class message_values_iterator {
public:
  using iterator_category = std::input_iterator_tag;
  using value_type = id_and_value;
  using difference_type = std::ptrdiff_t;

public:
  message_values_iterator();
  explicit message_values_iterator(std::span<const uint8_t> buffer);

  value_type operator*() const;

  std::add_pointer_t<value_type const> operator->() const;

  message_values_iterator& operator++();

  message_values_iterator operator++(int);

  friend bool operator==(const message_values_iterator& lhs,
                         const message_values_iterator& rhs);

  friend bool operator!=(const message_values_iterator& lhs,
                         const message_values_iterator& rhs);

private:
  void update_data();

  id_and_value _data;
  std::span<const uint8_t> _bytes;
};

} // namespace spymarine
