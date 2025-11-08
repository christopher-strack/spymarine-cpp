#include "spymarine/message_value.hpp"
#include "spymarine/byte_operations.hpp"

#include <algorithm>

namespace spymarine {

numeric_value::numeric_value(std::span<const uint8_t, 4> bytes) {
  std::copy(bytes.begin(), bytes.end(), _bytes.begin());
}

int16_t numeric_value::first() const {
  return to_uint16(std::span{_bytes}.subspan<0, 2>());
}

int16_t numeric_value::second() const {
  return to_uint16(std::span{_bytes}.subspan<2, 2>());
}

int32_t numeric_value::number() const {
  return (_bytes[0] << 24) | (_bytes[1] << 16) | (_bytes[2] << 8) | _bytes[3];
}

} // namespace spymarine
