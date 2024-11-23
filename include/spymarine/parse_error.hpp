#pragma once

namespace spymarine {

enum class parse_error {
  unknown_message,
  invalid_header,
  invalid_data_length,
  invalid_crc,
  invalid_device_message,
  invalid_device_count_message,
  invalid_device_type,
};

}
