#include "spymarine/parse_message.hpp"

#include <algorithm>
#include <print>

namespace spymarine {

namespace {

uint16_t to_uint16(const std::span<const uint8_t, 2> data) {
  return uint16_t((data[0] << 8) | data[1]);
}

std::optional<message_type> parse_message_type(uint8_t type) {
  switch (type) {
  case uint8_t(message_type::device_count):
    return message_type::device_count;
  case uint8_t(message_type::device_info):
    return message_type::device_info;
  case uint8_t(message_type::sensor_state):
    return message_type::sensor_state;
  }
  return std::nullopt;
};
} // namespace

std::expected<header, parse_error>
parse_header(const std::span<const uint8_t> data) {
  if (data.size() < header_size) {
    return std::unexpected{parse_error::invalid_data_length};
  }

  const std::array<uint8_t, 6> prefix = {0x00, 0x00, 0x00, 0x00, 0x00, 0xff};
  if (!std::ranges::equal(data.subspan(0, prefix.size()), prefix)) {
    return std::unexpected{parse_error::invalid_header};
  }

  if (data[13] != 0xff) {
    return std::unexpected{parse_error::invalid_header};
  }

  const auto type = data.data()[6];
  const auto length = to_uint16(data.subspan<11, 2>());

  return header{type, length};
}

uint16_t crc(const std::span<const uint8_t> bytes) {
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

std::expected<message, parse_error>
parse_message(const std::span<const uint8_t> data) {
  return parse_header(data).and_then(
      [&](const header header) -> std::expected<message, parse_error> {
        const auto type = parse_message_type(header.type);
        if (!type) {
          return std::unexpected{parse_error::unknown_message};
        }

        const auto data_length = data.size() - header_size + 1;
        if (header.length != data_length) {
          return std::unexpected{parse_error::invalid_data_length};
        }

        const auto calculated_crc =
            crc(std::span{data.begin() + 1, data.end() - 3});
        const auto received_crc =
            to_uint16(std::span<const uint8_t, 2>{data.end() - 2, 2});

        if (calculated_crc != received_crc) {
          return std::unexpected{parse_error::invalid_crc};
        }

        return message{*type,
                       std::span{data.begin() + header_size, data.end() - 2}};
      });
}

} // namespace spymarine
