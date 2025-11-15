#pragma once

#include "spymarine/crc.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/parse_message_header.hpp"
#include "spymarine/parse_message_type.hpp"

#include <cstdint>
#include <expected>
#include <span>

namespace spymarine {

constexpr std::expected<message, error>
parse_message(const std::span<const uint8_t> bytes) noexcept {
  return parse_header(bytes).and_then(
      [&](const header header) -> std::expected<message, error> {
        const auto type = parse_message_type(header.type);
        if (!type) {
          return std::unexpected{parse_error::unknown_message};
        }

        const auto data_length = bytes.size() - header_size + 1;
        if (header.length != data_length) {
          return std::unexpected{parse_error::invalid_data_length};
        }

        const auto calculated_crc = crc(bytes.subspan(1, bytes.size() - 4));
        const auto received_crc =
            to_uint16(std::span<const uint8_t, 2>{bytes.end() - 2, 2});

        if (calculated_crc != received_crc) {
          return std::unexpected{parse_error::invalid_crc};
        }

        return message{
            *type, bytes.subspan(header_size, bytes.size() - 2 - header_size)};
      });
}

} // namespace spymarine
