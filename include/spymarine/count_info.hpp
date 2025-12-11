#pragma once

#include "spymarine/id.hpp"

namespace spymarine {

struct count_info {
  device_id device_count;
  sensor_id sensor_count;

  auto operator<=>(const count_info&) const = default;
};

} // namespace spymarine
