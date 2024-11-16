#include "spymarine/message.hpp"

#include <algorithm>

namespace spymarine {

bool operator==(const message& lhs, const message& rhs) {
  return lhs.type == rhs.type && std::ranges::equal(lhs.data, rhs.data);
}

bool operator!=(const message& lhs, const message& rhs) {
  return !(lhs == rhs);
}

} // namespace spymarine
