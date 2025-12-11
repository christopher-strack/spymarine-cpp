#pragma once

#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/system_info.hpp"

#include <expected>

namespace spymarine {

constexpr std::expected<system_info, error>
parse_system_info(const message& msg) noexcept {
  if (msg.type() == message_type::system_information) {
    const auto serial_number_value = msg.values().find<numeric_value1>(1);
    const auto fw_version_value = msg.values().find<numeric_value1>(2);
    if (serial_number_value && fw_version_value) {
      return system_info{
          .serial_number = serial_number_value->uint32(),
          .fw_version =
              firmware_version{
                  .major = fw_version_value->low_uint16(),
                  .minor = fw_version_value->high_uint16(),
              },
      };
    }
  }
  return std::unexpected{parse_error::invalid_system_message};
}

} // namespace spymarine
