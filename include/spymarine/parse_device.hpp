#pragma once

#include "spymarine/device.hpp"
#include "spymarine/error.hpp"

#include <expected>
#include <span>
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

uint8_t sensor_state_offset(const parsed_device& device);

std::expected<parsed_device, error>
parse_device(const std::span<const uint8_t> bytes, uint8_t state_start_index);

} // namespace spymarine
