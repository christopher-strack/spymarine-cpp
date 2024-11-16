#include "spymarine/value_view.hpp"

#include <algorithm>

namespace spymarine {

value_view::value_view(std::span<const uint8_t> buffer) : _buffer{buffer} {}

value_iterator value_view::begin() const { return value_iterator(_buffer); }
value_iterator value_view::end() const {
  return value_iterator(std::span<const uint8_t>{});
}

value_iterator find_value(const uint8_t id, const value_view& values) {
  return std::ranges::find_if(
      values, [id](const auto value) { return value.id == id; });
}

} // namespace spymarine
