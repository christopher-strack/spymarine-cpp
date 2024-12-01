#pragma once

#include "spymarine/defaults.hpp"
#include "spymarine/error.hpp"

#include <cstdint>
#include <expected>

namespace spymarine {

std::expected<uint32_t, error>
discover(uint16_t port = simarine_default_udp_port);

} // namespace spymarine
