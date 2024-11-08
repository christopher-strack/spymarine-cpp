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
  if (data.size() < header_size) {
    return std::nullopt;
  }

  if (std::memcmp(data.data(), "\x00\x00\x00\x00\x00\xff", 6) != 0) {
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
  const auto data_length = raw_response.size() - header_size + 1;

  if (header->length != data_length) {
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
      std::span{raw_response.begin() + header_size, raw_response.end() - 2}};
}

namespace {
std::array<uint8_t, 2> to_bytes(uint16_t value) {
  return {uint8_t((value >> 8) & 0xff), uint8_t(value & 0xff)};
}
} // namespace

std::span<uint8_t> make_request(message_type type,
                                const std::span<const uint8_t> data,
                                std::span<uint8_t> buffer) {
  const auto payload_size = header_size + data.size();
  const auto total_size = payload_size + 2;

  if (buffer.size() < total_size) {
    throw std::out_of_range("Buffer is too small for request");
  }

  const auto length = to_bytes(3 + data.size());
  const auto header = std::array<uint8_t, header_size>{
      0x00, 0x00, 0x00, 0x00, 0x00,      0xff,      uint8_t(type),
      0x04, 0x8c, 0x55, 0x4b, length[0], length[1], 0xff};

  auto it = std::copy(header.begin(), header.end(), buffer.begin());
  it = std::copy(data.begin(), data.end(), it);

  const auto calculated_crc =
      to_bytes(crc(std::span{buffer.begin() + 1, payload_size - 2}));
  std::copy(calculated_crc.begin(), calculated_crc.end(), it);

  return std::span{buffer.begin(), total_size};
}

std::span<uint8_t> make_device_request(uint8_t device_id,
                                       std::span<uint8_t> buffer) {
  const std::array<uint8_t, 19> data{
      0x00, 0x01, 0x00, 0x00, 0x00, device_id, 0xff, 0x01, 0x03, 0x00,
      0x00, 0x00, 0x00, 0xff, 0x00, 0x00,      0x00, 0x00, 0xff};
  return make_request(message_type::device_info, data, buffer);
}

numeric_value::numeric_value(std::span<const uint8_t, 4> bytes) {
  std::copy(bytes.begin(), bytes.end(), _bytes.begin());
}

uint16_t numeric_value::first() const { return (_bytes[0] << 8) | _bytes[1]; }

uint16_t numeric_value::second() const { return (_bytes[2] << 8) | _bytes[3]; }

uint32_t numeric_value::number() const {
  return (_bytes[0] << 24) | (_bytes[1] << 16) | (_bytes[2] << 8) | _bytes[3];
}

namespace {

std::optional<value> read_value1(std::span<const uint8_t> bytes) {
  if (bytes.size() >= 4) {
    return value{numeric_value{bytes.subspan<0, 4>()}};
  }
  return std::nullopt;
}

std::optional<value> read_value3(std::span<const uint8_t> bytes) {
  if (bytes.size() >= 9) {
    return value{numeric_value{bytes.subspan<5, 4>()}};
  }
  return std::nullopt;
}

std::optional<std::string_view> read_string(std::span<const uint8_t> bytes) {
  if (bytes.size() >= 5) {
    const auto data = bytes.subspan(5);
    const auto it = std::find(data.begin(), data.end(), 0);
    if (it != data.end()) {
      const auto size = static_cast<size_t>(std::distance(data.begin(), it));
      return std::string_view{reinterpret_cast<const char*>(data.data()), size};
    }
  }
  return std::nullopt;
}

std::span<const uint8_t> advance_bytes(std::span<const uint8_t> bytes,
                                       size_t n) {
  return bytes.size() >= n ? bytes.subspan(n) : std::span<const uint8_t>{};
}

std::optional<value> find_value(const uint8_t id,
                                std::span<const uint8_t> bytes) {
  while (bytes.size() >= 2) {
    const auto value_id = bytes[0];
    const auto value_type = bytes[1];
    bytes = bytes.subspan(2);

    if (value_type == 1) {
      if (value_id == id) {
        return read_value1(bytes);
      }
      bytes = advance_bytes(bytes, 5);
    } else if (value_type == 3) {
      if (value_id == id) {
        return read_value3(bytes);
      }
      bytes = advance_bytes(bytes, 10);
    } else if (value_type == 4) {
      const auto string = read_string(bytes);
      if (value_id == id) {
        return string;
      } else if (!string) {
        return std::nullopt;
      }
      bytes = advance_bytes(bytes, string->size() + 7);
    }
  }

  return std::nullopt;
}

std::optional<numeric_value>
find_numeric_value(const uint8_t id, std::span<const uint8_t> bytes) {
  if (const auto value = find_value(id, bytes)) {
    if (const auto n = std::get_if<numeric_value>(&*value)) {
      return *n;
    }
  }
  return std::nullopt;
}

std::optional<std::string_view> find_string(const uint8_t id,
                                            std::span<const uint8_t> bytes) {
  if (const auto value = find_value(id, bytes)) {
    if (const auto str = std::get_if<std::string_view>(&*value)) {
      return *str;
    }
  }
  return std::nullopt;
}

} // namespace

std::ostream& operator<<(std::ostream& stream,
                         const device_properties& properties) {
  stream << "{";
  for (auto it = properties.begin(); it != properties.end(); ++it) {
    stream << it->first << "=";
    std::visit(
        [&](const auto& value) -> std::ostream& { return stream << value; },
        it->second);
    if (std::next(it) != properties.end()) {
      stream << ",";
    }
  }
  stream << "}";
  return stream;
}

bool operator==(const device& lhs, const device& rhs) {
  const auto result = lhs.name == rhs.name && lhs.type == rhs.type &&
                      lhs.properties == rhs.properties;
  return result;
}

bool operator!=(const device& lhs, const device& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& stream, const device& device) {
  stream << "device<type=" << int(device.type) << ",name=" << device.name
         << ",properties=" << device.properties << ">";
  return stream;
}

namespace {
std::string fluid_type_string(uint16_t fluid_type) {
  switch (fluid_type) {
  case 1:
    return "fresh_water";
  case 2:
    return "fuel";
  case 3:
    return "waste_water";
  }
  return "unknown";
}

std::string battery_type_string(uint16_t battery_type) {
  switch (battery_type) {
  case 1:
    return "wet_low_maintenance";
  case 2:
    return "wet_maintenance_free";
  case 3:
    return "agm";
  case 4:
    return "deep_cycle";
  case 5:
    return "gel";
  case 6:
    return "lifepo4";
  }
  return "unknown";
}
} // namespace

device make_device(const std::span<const uint8_t> bytes) {
  device device;
  if (const auto name = find_string(3, bytes)) {
    device.name = *name;
  }
  const auto type = find_numeric_value(1, bytes).value().second();

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
    device.properties["fluid_type"] =
        fluid_type_string(find_numeric_value(6, bytes).value().second());
    device.properties["capacity"] =
        find_numeric_value(7, bytes).value().second() / 10.0;
  } else if (type == 9) {
    device.type = device_type::battery;
    device.properties["battery_type"] =
        battery_type_string(find_numeric_value(8, bytes).value().second());
    device.properties["capacity"] =
        find_numeric_value(5, bytes).value().second() / 100.0;
  } else {
    device.type = device_type::unknown;
  }

  return device;
}

} // namespace spymarine
