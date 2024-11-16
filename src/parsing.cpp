#include "spymarine/parsing.hpp"

#include <algorithm>
#include <expected>

namespace spymarine {

bool operator==(const message& lhs, const message& rhs) {
  return lhs.type == rhs.type && std::ranges::equal(lhs.data, rhs.data);
}

bool operator!=(const message& lhs, const message& rhs) {
  return !(lhs == rhs);
}

namespace {

uint16_t to_uint16(const std::span<const uint8_t, 2> data) {
  return uint16_t((data[0] << 8) | data[1]);
}

} // namespace

std::expected<header, error> parse_header(const std::span<const uint8_t> data) {
  if (data.size() < header_size) {
    return std::unexpected{error::invalid_data_length};
  }

  const std::array<uint8_t, 6> prefix = {0x00, 0x00, 0x00, 0x00, 0x00, 0xff};
  if (!std::ranges::equal(data.subspan(0, prefix.size()), prefix)) {
    return std::unexpected{error::invalid_header};
  }

  if (data[13] != 0xff) {
    return std::unexpected{error::invalid_header};
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

std::expected<message, error>
parse_message(const std::span<const uint8_t> data) {
  return parse_header(data).and_then(
      [&](const auto header) -> std::expected<message, error> {
        const auto data_length = data.size() - header_size + 1;

        if (header.length != data_length) {
          return std::unexpected{error::invalid_data_length};
        }

        const auto calculated_crc =
            crc(std::span{data.begin() + 1, data.end() - 3});
        const auto received_crc =
            to_uint16(std::span<const uint8_t, 2>{data.end() - 2, 2});

        if (calculated_crc != received_crc) {
          return std::unexpected{error::invalid_crc};
        }

        return message{static_cast<message_type>(header.type),
                       std::span{data.begin() + header_size, data.end() - 2}};
      });
}

} // namespace spymarine
