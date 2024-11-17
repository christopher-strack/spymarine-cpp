#include "spymarine/message_values_view.hpp"

#include <algorithm>

namespace spymarine {

message_values_view::message_values_view(std::span<const uint8_t> buffer)
    : _buffer{buffer} {}

message_values_iterator message_values_view::begin() const {
  return message_values_iterator(_buffer);
}
message_values_iterator message_values_view::end() const {
  return message_values_iterator(std::span<const uint8_t>{});
}

message_values_iterator find_value(const uint8_t id,
                                   const message_values_view& values) {
  return std::ranges::find_if(
      values, [id](const auto value) { return value.id == id; });
}

} // namespace spymarine
