#pragma once

#include <compare> // IWYU pragma: keep
#include <cstdint>

namespace spymarine {

struct firmware_version {
  uint16_t major;
  uint16_t minor;

  auto operator<=>(const firmware_version&) const = default;
};

struct system_info {
  uint32_t serial_number;
  firmware_version fw_version;

  auto operator<=>(const system_info&) const = default;
};

} // namespace spymarine
