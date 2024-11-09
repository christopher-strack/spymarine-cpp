#include "spymarine/parsing.hpp"

namespace spymarine {

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

namespace {
fluid_type to_fluid_type(const uint16_t type) {
  switch (type) {
  case 1:
    return fluid_type::fresh_water;
  case 2:
    return fluid_type::fuel;
  case 3:
    return fluid_type::waste_water;
  }
  return fluid_type::unknown;
}

battery_type to_battery_type(const uint16_t battery_type) {
  switch (battery_type) {
  case 1:
    return battery_type::wet_low_maintenance;
  case 2:
    return battery_type::wet_maintenance_free;
  case 3:
    return battery_type::agm;
  case 4:
    return battery_type::deep_cycle;
  case 5:
    return battery_type::gel;
  case 6:
    return battery_type::lifepo4;
  }
  return battery_type::unknown;
}
} // namespace

std::optional<device> parse_device(const std::span<const uint8_t> bytes,
                                   const uint8_t sensor_start_index) {
  const auto type_value = find_numeric_value(1, bytes);
  if (!type_value) {
    return std::nullopt;
  }

  const auto type = type_value->second();
  const auto name = find_string(3, bytes);

  if (type == 0) {
    return null_device{};
  } else if (type == 1) {
    if (name == "PICO INTERNAL") {
      return pico_internal_device{sensor_start_index};
    } else if (name) {
      return device{voltage_device{std::string{*name}, sensor_start_index}};
    }
  } else if (type == 2) {
    if (name) {
      return device{current_device{std::string{*name}, sensor_start_index}};
    }
  } else if (type == 3) {
    if (name) {
      return device{temperature_device{std::string{*name}, sensor_start_index}};
    }
  } else if (type == 5) {
    if (name) {
      return device{barometer_device{std::string{*name}, sensor_start_index}};
    }
  } else if (type == 6) {
    if (name) {
      return device{resistive_device{std::string{*name}, sensor_start_index}};
    }
  } else if (type == 8) {
    const auto fluid_type = find_numeric_value(6, bytes);
    const auto capacity = find_numeric_value(7, bytes);
    if (name && fluid_type && capacity) {
      return device{tank_device{
          std::string{*name},
          to_fluid_type(fluid_type->second()),
          capacity->second() / 10.0f,
          sensor_start_index,
      }};
    }
  } else if (type == 9) {
    const auto battery_type = find_numeric_value(8, bytes);
    const auto capacity = find_numeric_value(5, bytes);
    if (name && battery_type && capacity) {
      return device{battery_device{
          std::string{*name},
          to_battery_type(battery_type->second()),
          capacity->second() / 100.0f,
          sensor_start_index,
      }};
    }
  }

  return unknown_device{};
}

} // namespace spymarine
