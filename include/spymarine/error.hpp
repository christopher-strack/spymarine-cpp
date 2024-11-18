#pragma once

#include "spymarine/parse_error.hpp"

#include <string>
#include <system_error>
#include <variant>

namespace spymarine {

using error = std::variant<parse_error, std::error_code>;

error error_from_parse_error(const parse_error& err);
error error_from_error_code(const std::error_code& ec);

std::string error_message(error error);

} // namespace spymarine
