#include "spymarine/parsing.hpp"

#include <ostream>

namespace spymarine {

bool operator==(const Header &lhs, const Header &rhs) {
  return lhs.type == rhs.type && lhs.length == rhs.length;
}

bool operator!=(const Header &lhs, const Header &rhs) { return !(lhs == rhs); }

bool operator==(const Message &lhs, const Message &rhs) {
  return lhs.type == rhs.type &&
         std::memcmp(lhs.data.data(), rhs.data.data(),
                     std::min(lhs.data.size(), rhs.data.size())) == 0;
}

bool operator!=(const Message &lhs, const Message &rhs) {
  return !(lhs == rhs);
}

namespace {

uint16_t toUInt16(const std::span<const uint8_t, 2> data) {
  return uint16_t((data[0] << 8) | data[1]);
}

} // namespace

std::optional<Header> parseHeader(const std::span<const uint8_t> data) {
  if (data.size() < headerLength) {
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
  const auto length = toUInt16(data.subspan<11, 2>());

  return Header{type, length};
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

std::optional<Message>
parseResponse(const std::span<const uint8_t> rawResponse) {
  const auto header = parseHeader(rawResponse);
  const auto dataLength = rawResponse.size() - headerLength + 1;

  if (header->length != dataLength) {
    return std::nullopt;
  }

  const auto calculatedCrc =
      crc(std::span{rawResponse.begin() + 1, rawResponse.end() - 3});
  const auto receivedCrc =
      toUInt16(std::span<const uint8_t, 2>{rawResponse.end() - 2, 2});

  if (calculatedCrc != receivedCrc) {
    return std::nullopt;
  }

  return Message{
      static_cast<MessageType>(header->type),
      std::span{rawResponse.begin() + headerLength, rawResponse.end() - 2}};
}

namespace {
int32_t bytesToNumber(std::span<const uint8_t, 4> bytes) {
  return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

std::span<const uint8_t> parseValue(const std::span<const uint8_t> bytes,
                                    ValueMap &dict) {
  if (bytes.size() < 2) {
    // raise ParsingError("Couldn't parse property")
    return {};
  }

  const auto propertyId = bytes[0];
  const auto propertyType = bytes[1];
  if (propertyType == 1) {
    // TODO check for range
    dict.numbers.insert(
        std::pair{propertyId, bytesToNumber(bytes.subspan<2, 4>())});
    return bytes.subspan(7);
  } else if (propertyType == 3) {
    // TODO check for range
    dict.numbers.insert(
        std::pair{propertyId, bytesToNumber(bytes.subspan<7, 4>())});
    return bytes.subspan(12);
  } else if (propertyType == 4) {
    const auto data = bytes.subspan(7);
    const auto it = std::find(data.begin(), data.end(), 0);
    if (it != data.end()) {
      const auto pos = std::distance(data.begin(), it);

      // It looks like a custom encoding is used for strings. Special
      // characters will unfortunately not works as expected.
      dict.strings.insert(
          std::pair{propertyId, std::string{data.begin(), data.begin() + pos}});

      return bytes.subspan(7 + pos + 2);
    }
  }

  // raise ParsingError(f"Unknown property type {property_type}")
  return {};
}
} // namespace

ValueMap parseValueMap(std::span<const uint8_t> bytes) {
  ValueMap dict;

  while (!bytes.empty()) {
    bytes = parseValue(bytes, dict);
  }

  return dict;
}

bool operator==(const Device &lhs, const Device &rhs) {
  const auto result = lhs.name == rhs.name && lhs.type == rhs.type;
  return result;
}

bool operator!=(const Device &lhs, const Device &rhs) { return !(lhs == rhs); }

std::ostream &operator<<(std::ostream &stream, const Device &device) {
  stream << "Device<type=" << int(device.type) << ",name=" << device.name
         << ">";
  return stream;
}

Device deviceFromValueMap(const ValueMap &map) {
  Device device;
  if (map.strings.count(3)) {
    device.name = map.strings.at(3);
  }
  const auto deviceType = map.numbers.at(1);

  if (deviceType == 0) {
    device.type = DeviceType::null;
  } else if (deviceType == 1) {
    if (device.name == "PICO INTERNAL") {
      device.type = DeviceType::picoInternal;
    } else {
      device.type = DeviceType::voltage;
    }
  } else if (deviceType == 2) {
    device.type = DeviceType::current;
  } else if (deviceType == 3) {
    device.type = DeviceType::temperature;
  } else if (deviceType == 5) {
    device.type = DeviceType::barometer;
  } else if (deviceType == 6) {
    device.type = DeviceType::resistive;
  } else if (deviceType == 8) {
    device.type = DeviceType::tank;
  } else if (deviceType == 9) {
    device.type = DeviceType::battery;
  } else {
    device.type = DeviceType::unknown;
  }

  return device;
}

} // namespace spymarine
