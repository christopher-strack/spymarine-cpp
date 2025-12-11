#pragma once

#include "spymarine/count_info.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/message_value.hpp"

#include <cstdint>
#include <expected>

namespace spymarine {

constexpr std::expected<count_info, error>
parse_count_info(const message& msg) noexcept {
  if (msg.type() == message_type::count_information) {
    const auto device_count = msg.values().find<numeric_value1>(1);
    const auto sensor_count = msg.values().find<numeric_value1>(2);
    if (device_count && sensor_count) {
      return count_info{
          .device_count = uint8_t(device_count->uint8() + 1),
          .sensor_count = uint8_t(sensor_count->uint8() + 1),
      };
    }
  }
  return std::unexpected{parse_error::invalid_count_message};
}

} // namespace spymarine
