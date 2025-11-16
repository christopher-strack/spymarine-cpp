#pragma once

#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message_values_view.hpp"

#include <cstdint>
#include <expected>
#include <variant>

namespace spymarine {
namespace detail {
template <typename T, typename... Args> struct variant_extender;

template <typename... Args0, typename... Args1>
struct variant_extender<std::variant<Args0...>, Args1...> {
  using type = std::variant<Args0..., Args1...>;
};
} // namespace detail

struct null_device {
  auto operator<=>(const null_device&) const = default;
};

struct unknown_device {
  auto operator<=>(const unknown_device&) const = default;
};

using parsed_device =
    detail::variant_extender<device, null_device, unknown_device>::type;

uint8_t sensor_state_offset(const parsed_device& d);

std::expected<parsed_device, error>
parse_device(const message_values_view values, uint8_t state_start_index);

} // namespace spymarine
