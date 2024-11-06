#include "spymarine/parsing.hpp"

#include <ostream>

namespace spymarine {

bool operator==(const header& lhs, const header& rhs) {
  return lhs.type == rhs.type && lhs.length == rhs.length;
}

bool operator!=(const header& lhs, const header& rhs) { return !(lhs == rhs); }

bool operator==(const message& lhs, const message& rhs) {
  return lhs.type == rhs.type &&
         std::memcmp(lhs.data.data(), rhs.data.data(),
                     std::min(lhs.data.size(), rhs.data.size())) == 0;
}

bool operator!=(const message& lhs, const message& rhs) {
  return !(lhs == rhs);
}

namespace {

uint16_t to_uint16(const std::span<const uint8_t, 2> data) {
  return uint16_t((data[0] << 8) | data[1]);
}

} // namespace

std::optional<header> parse_header(const std::span<const uint8_t> data) {
  if (data.size() < header_length) {
    return std::nullopt;
  }

  if (std::memcmp(data.data(), "\x00\x00\x00\x00\x00\xff", 6) != 0) {
    return std::nullopt;
  }

  if (std::memcmp(data.data() + 7, "\x85\xde\xc3\x46", 4) != 0) {
    return std::nullopt;
  }

  if (data[13] != 0xff) {
    return std::nullopt;
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

std::optional<message>
parse_response(const std::span<const uint8_t> raw_response) {
  const auto header = parse_header(raw_response);
  const auto dataLength = raw_response.size() - header_length + 1;

  if (header->length != dataLength) {
    return std::nullopt;
  }

  const auto calculated_crc =
      crc(std::span{raw_response.begin() + 1, raw_response.end() - 3});
  const auto received_crc =
      to_uint16(std::span<const uint8_t, 2>{raw_response.end() - 2, 2});

  if (calculated_crc != received_crc) {
    return std::nullopt;
  }

  return message{
      static_cast<message_type>(header->type),
      std::span{raw_response.begin() + header_length, raw_response.end() - 2}};
}

uint16_t property_value::first() const { return (bytes[0] << 8) | bytes[1]; }

uint16_t property_value::second() const { return (bytes[2] << 8) | bytes[3]; }

uint32_t property_value::number() const {
  return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

namespace {
property_value make_property_value(std::span<const uint8_t, 4> bytes) {
  property_value value;
  std::copy(bytes.begin(), bytes.end(), value.bytes.begin());
  return value;
}

std::span<const uint8_t> parse_value(const std::span<const uint8_t> bytes,
                                     property_dict& dict) {
  if (bytes.size() < 2) {
    // raise ParsingError("Couldn't parse property")
    return {};
  }

  const auto property_id = bytes[0];
  const auto property_type = bytes[1];
  if (property_type == 1) {
    // TODO check for range
    dict.numbers.insert(
        std::pair{property_id, make_property_value(bytes.subspan<2, 4>())});
    return bytes.subspan(7);
  } else if (property_type == 3) {
    // TODO check for range
    dict.numbers.insert(
        std::pair{property_id, make_property_value(bytes.subspan<7, 4>())});
    return bytes.subspan(12);
  } else if (property_type == 4) {
    const auto data = bytes.subspan(7);
    const auto it = std::find(data.begin(), data.end(), 0);
    if (it != data.end()) {
      const auto size = static_cast<size_t>(std::distance(data.begin(), it));

      // It looks like a custom encoding is used for strings. Special
      // characters will unfortunately not works as expected.
      dict.strings.insert(std::pair{
          property_id,
          std::string_view{reinterpret_cast<const char*>(data.data()), size}});

      return bytes.subspan(7 + size + 2);
    }
  }

  // raise ParsingError(f"Unknown property type {property_type}")
  return {};
}
} // namespace

property_dict parse_property_dict(std::span<const uint8_t> bytes) {
  property_dict dict;

  while (!bytes.empty()) {
    bytes = parse_value(bytes, dict);
  }

  return dict;
}

bool operator==(const device& lhs, const device& rhs) {
  const auto result = lhs.name == rhs.name && lhs.type == rhs.type;
  return result;
}

bool operator!=(const device& lhs, const device& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& stream, const device& device) {
  stream << "device<type=" << int(device.type) << ",name=" << device.name
         << ">";
  return stream;
}

device device_from_property_dict(const property_dict& map) {
  device device;
  if (map.strings.count(3)) {
    device.name = map.strings.at(3);
  }
  const auto type = map.numbers.at(1).second();

  if (type == 0) {
    device.type = device_type::null;
  } else if (type == 1) {
    if (device.name == "PICO INTERNAL") {
      device.type = device_type::pico_internal;
    } else {
      device.type = device_type::voltage;
    }
  } else if (type == 2) {
    device.type = device_type::current;
  } else if (type == 3) {
    device.type = device_type::temperature;
  } else if (type == 5) {
    device.type = device_type::barometer;
  } else if (type == 6) {
    device.type = device_type::resistive;
  } else if (type == 8) {
    device.type = device_type::tank;
  } else if (type == 9) {
    device.type = device_type::battery;
  } else {
    device.type = device_type::unknown;
  }

  return device;
}

} // namespace spymarine
