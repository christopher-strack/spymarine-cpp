#pragma once

#include "spymarine/parse_error.hpp"

#include <string>
#include <system_error>
#include <variant>

namespace spymarine {

using error = std::variant<parse_error, std::error_code>;

std::string error_message(error error);

} // namespace spymarine
