#include "spymarine/parsing.hpp"

#include <algorithm>
#include <expected>
#include <iostream>
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

std::optional<std::string_view> read_string(std::span<const uint8_t> bytes) {
  if (bytes.size() >= 5) {
    const auto data = bytes.subspan(5);
    const auto it = std::ranges::find(data, 0);
    if (it != data.end()) {
      const auto size = static_cast<size_t>(std::distance(data.begin(), it));
      return std::string_view{reinterpret_cast<const char*>(data.data()), size};
    }
  }
  return std::nullopt;
}

std::span<const uint8_t> advance_bytes(std::span<const uint8_t> bytes,
                                       size_t n) {
  return bytes.size() >= n + 2 ? bytes.subspan(n) : std::span<const uint8_t>{};
}

class value_view : public std::ranges::view_interface<value_view> {
public:
  value_view() = default;
  value_view(std::span<const uint8_t> buffer) : _buffer{buffer} {}

  class iterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::tuple<uint8_t, value>;
    using difference_type = std::ptrdiff_t;

  public:
    iterator() = default;
    explicit iterator(std::span<const uint8_t> buffer) : _remaining(buffer) {}

    value_type operator*() const {
      const auto value_id = _remaining[0];
      const auto value_type = _remaining[1];
      const auto bytes = _remaining.subspan(2);

      if (value_type == 1 && bytes.size() >= 4) {
        return {value_id, numeric_value{bytes.subspan<0, 4>()}};
      } else if (value_type == 3 && bytes.size() >= 9) {
        return {value_id, numeric_value{bytes.subspan<5, 4>()}};
      } else if (value_type == 4) {
        if (const auto string = read_string(bytes)) {
          return std::tuple{value_id, *string};
        }
      }

      return std::tuple{value_id, invalid_value{}};
    }

    iterator& operator++() {
      if (_remaining.size() < 2) {
        _remaining = {};
        return *this;
      }

      const auto value_type = _remaining[1];
      const auto bytes = _remaining.subspan(2);

      if (value_type == 1) {
        _remaining = advance_bytes(bytes, 5);
      } else if (value_type == 3) {
        _remaining = advance_bytes(bytes, 10);
      } else if (value_type == 4) {
        const auto string = read_string(bytes);
        _remaining = string ? advance_bytes(bytes, string->size() + 7)
                            : std::span<const uint8_t>{};
      } else {
        _remaining = {};
      }
      return *this;
    }

    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const iterator& lhs, const iterator& rhs) {
      return std::ranges::equal(lhs._remaining, rhs._remaining);
    }

    friend bool operator!=(const iterator& lhs, const iterator& rhs) {
      return !(lhs == rhs);
    }

  private:
    std::span<const uint8_t> _remaining;
  };

  iterator begin() const { return iterator(_buffer); }

  iterator end() const { return iterator(std::span<const uint8_t>{}); }

private:
  std::span<const uint8_t> _buffer;
};

std::optional<value> find_value(const uint8_t expected_id,
                                const value_view& values) {
  if (const auto it = std::ranges::find_if(
          values,
          [&](const auto value) { return std::get<0>(value) == expected_id; });
      it != values.end()) {
    return std::get<1>(*it);
  }

  return std::nullopt;
}

template <typename T>
std::optional<T> find_value_for_type(const uint8_t id,
                                     const value_view& values) {
  if (const auto value = find_value(id, values)) {
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

std::expected<parsed_device, error>
parse_device(const std::span<const uint8_t> bytes,
             const uint8_t state_start_index) {
  const auto values = value_view{bytes};
  const auto type_value = find_value_for_type<numeric_value>(1, values);
  if (!type_value) {
    return std::unexpected{error::invalid_device_message};
  }

  const auto type = type_value->second();
  const auto name_value = find_value_for_type<std::string_view>(3, values);

  switch (type) {
  case 0:
    return null_device{};
  case 1:
    return name_value == "PICO INTERNAL"
               ? parsed_device{pico_internal_device{state_start_index}}
               : parsed_device{voltage_device{std::string{*name_value},
                                              state_start_index}};
  case 2:
    if (name_value) {
      return current_device{std::string{*name_value}, state_start_index};
    }
    break;
  case 3:
    if (name_value) {
      return temperature_device{std::string{*name_value}, state_start_index};
    }
    break;
  case 4:
    return unknown_device{};
  case 5:
    if (name_value) {
      return barometer_device{std::string{*name_value}, state_start_index};
    }
    break;
  case 6:
    if (name_value) {
      return resistive_device{std::string{*name_value}, state_start_index};
    }
    break;
  case 7:
    return unknown_device{};
  case 8: {
    const auto fluid_type = find_value_for_type<numeric_value>(6, bytes);
    const auto capacity = find_value_for_type<numeric_value>(7, bytes);
    if (name_value && fluid_type && capacity) {
      return tank_device{
          std::string{*name_value},
          to_fluid_type(fluid_type->second()),
          capacity->second() / 10.0f,
          state_start_index,
      };
    }
    break;
  }
  case 9: {
    const auto battery_type = find_value_for_type<numeric_value>(8, bytes);
    const auto capacity = find_value_for_type<numeric_value>(5, bytes);
    if (name_value && battery_type && capacity) {
      return battery_device{
          std::string{*name_value},
          to_battery_type(battery_type->second()),
          capacity->second() / 100.0f,
          state_start_index,
      };
    }
    break;
  }
  case 10:
    return unknown_device{};
  case 14:
    return unknown_device{};
  default:
    return std::unexpected{error::invalid_device_type};
  }

  return std::unexpected{error::invalid_device_message};
}

} // namespace spymarine
