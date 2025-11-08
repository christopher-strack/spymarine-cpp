#pragma once

#include "spymarine/byte_operations.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"

#include <algorithm>
#include <array>
#include <expected>
#include <optional>
#include <span>
#include <utility>

namespace spymarine {

constexpr auto header_size = 14;

struct header {
  uint8_t type;
  uint16_t length;

  constexpr auto operator<=>(const header&) const noexcept = default;
};

namespace detail {

constexpr std::optional<message_type>
parse_message_type(uint8_t type) noexcept {
  switch (type) {
  case std::to_underlying(message_type::device_count):
    return message_type::device_count;
  case std::to_underlying(message_type::device_info):
    return message_type::device_info;
  case std::to_underlying(message_type::sensor_state):
    return message_type::sensor_state;
  }
  return std::nullopt;
};

} // namespace detail

constexpr std::expected<header, error>
parse_header(const std::span<const uint8_t> bytes) noexcept {
  if (bytes.size() < header_size) {
    return std::unexpected{parse_error::invalid_data_length};
  }

  const auto header_bytes = bytes.subspan<0, header_size>();

  constexpr auto prefix =
      std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00, 0x00, 0xff});

  if (!std::ranges::equal(header_bytes.subspan<0, prefix.size()>(), prefix)) {
    return std::unexpected{parse_error::invalid_header};
  }

  if (header_bytes[13] != 0xff) {
    return std::unexpected{parse_error::invalid_header};
  }

  const auto type = header_bytes[6];
  const auto length = to_uint16(header_bytes.subspan<11, 2>());

  return header{type, length};
}

/* Original source: https://github.com/htool/pico2signalk
 * Copyright Erik Bosman / @brainsmoke
 */
constexpr uint16_t crc(const std::span<const uint8_t> bytes) noexcept {
  const uint16_t poly = 0x1189;
  uint16_t crc = 0;

  for (auto byte : bytes) {
    crc ^= byte << 8;
    for (int i = 0; i < 8; i++) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ poly;
      } else {
        crc <<= 1;
      }
    }
  }

  return crc;
}

constexpr std::expected<message, error>
parse_message(const std::span<const uint8_t> bytes) noexcept {
  return parse_header(bytes).and_then(
      [&](const header header) -> std::expected<message, error> {
        const auto type = detail::parse_message_type(header.type);
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
