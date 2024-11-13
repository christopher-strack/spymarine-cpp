#include "spymarine/parsing.hpp"

#include <algorithm>
#include <expected>
#include <ranges>

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

template <typename T>
std::optional<T> find_value_for_type(const uint8_t id,
                                     std::span<const uint8_t> bytes) {
  if (const auto value = find_value(id, bytes)) {
    if (const auto n = std::get_if<T>(&*value)) {
      return *n;
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

std::expected<device, error> parse_device(const std::span<const uint8_t> bytes,
                                          const uint8_t sensor_start_index) {
  const auto type_value = find_value_for_type<numeric_value>(1, bytes);
  if (!type_value) {
    return std::unexpected{error::invalid_device_message};
  }

  const auto type = type_value->second();
  const auto name = find_value_for_type<std::string_view>(3, bytes);

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
    const auto fluid_type = find_value_for_type<numeric_value>(6, bytes);
    const auto capacity = find_value_for_type<numeric_value>(7, bytes);
    if (name && fluid_type && capacity) {
      return device{tank_device{
          std::string{*name},
          to_fluid_type(fluid_type->second()),
          capacity->second() / 10.0f,
          sensor_start_index,
      }};
    }
  } else if (type == 9) {
    const auto battery_type = find_value_for_type<numeric_value>(8, bytes);
    const auto capacity = find_value_for_type<numeric_value>(5, bytes);
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
