#include "spymarine/device.hpp"

namespace spymarine {

size_t sensor_state_offset(const device& device) {
  return std::visit([](const auto& d) { return d.sensor_state_offset; },
                    device);
}

} // namespace spymarine
