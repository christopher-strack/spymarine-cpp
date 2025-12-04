#pragma once

#include "spymarine/message.hpp"

#include <cstdint>
#include <optional>
#include <utility>

namespace spymarine {

constexpr std::optional<message_type>
parse_message_type(uint8_t type) noexcept {
  switch (type) {
  case std::to_underlying(message_type::count_information):
    return message_type::count_information;
  case std::to_underlying(message_type::device_information):
    return message_type::device_information;
  case std::to_underlying(message_type::sensor_state):
    return message_type::sensor_state;
  }
  return std::nullopt;
};

} // namespace spymarine
