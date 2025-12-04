#pragma once

#include "spymarine/message_values_view.hpp"

#include <algorithm>
#include <cstdint>
#include <span>

namespace spymarine {

enum class message_type : uint8_t {
  // Request the number of connected devices and sensors
  count_information = 0x02,

  // Request information about a device
  device_information = 0x41,

  // Sensor update message (UDP)
  sensor_state = 0xb0,
};

class message {
public:
  constexpr message(message_type type, std::span<const uint8_t> data) noexcept
      : _type{type}, _data{data} {}

  constexpr message_type type() const noexcept { return _type; }

  constexpr message_values_view values() const noexcept {
    return message_values_view{_data};
  }

  friend constexpr bool operator==(const message& lhs,
                                   const message& rhs) noexcept {
    return lhs._type == rhs._type && std::ranges::equal(lhs._data, rhs._data);
  }

private:
  message_type _type;
  std::span<const uint8_t> _data;
};

} // namespace spymarine
