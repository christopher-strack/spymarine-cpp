#include "spymarine/error.hpp"
#include "spymarine/overloaded.hpp"

namespace spymarine {

error error_from_parse_error(const parse_error& err) { return err; }

error error_from_error_code(const std::error_code& ec) { return ec; }

std::string error_message(error err) {
  return std::visit(overloaded{
                        [](parse_error e) { return std::to_string(int(e)); },
                        [](std::error_code ec) { return ec.message(); },
                    },
                    err);
}

} // namespace spymarine
