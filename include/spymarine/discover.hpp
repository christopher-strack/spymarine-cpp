#pragma once

#include "spymarine/defaults.hpp"

#include <expected>
#include <system_error>

namespace spymarine {

std::expected<uint32_t, std::error_code>
discover(uint16_t port = simarine_default_udp_port);

} // namespace spymarine
