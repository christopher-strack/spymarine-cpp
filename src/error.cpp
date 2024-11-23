#include "spymarine/error.hpp"
#include "spymarine/overloaded.hpp"

namespace spymarine {

error error_from_parse_error(const parse_error& err) { return err; }

error error_from_error_code(const std::error_code& ec) { return ec; }

namespace {
std::string to_string(parse_error e) {
  switch (e) {
  case parse_error::invalid_header:
    return "invalid_header";
  case parse_error::invalid_data_length:
    return "invalid_data_length";
  case parse_error::invalid_crc:
    return "invalid_crc";
  case parse_error::invalid_device_message:
    return "invalid_device_message";
  case parse_error::invalid_device_count_message:
    return "invalid_device_count_message";
  case parse_error::invalid_device_type:
    return "invalid_device_type";
  }

  return "unknown";
}
} // namespace

std::string error_message(error err) {
  return std::visit(overloaded{
                        [](parse_error e) { return to_string(e); },
                        [](std::error_code ec) { return ec.message(); },
                    },
                    err);
}

} // namespace spymarine
