#pragma once

#include "spymarine/device2.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/overloaded.hpp"
#include "spymarine/parse_count_info.hpp"
#include "spymarine/sensor2.hpp"
#include "spymarine/system_info.hpp"

#include <catch2/catch_all.hpp>

#include <expected>
#include <format>

namespace Catch {
template <> struct StringMaker<std::vector<uint8_t>> {
  static std::string convert(const std::vector<uint8_t>& v) {
    std::string out;
    out += "[";
    for (size_t i = 0; i < v.size(); ++i) {
      out += std::to_string(static_cast<unsigned int>(v[i]));
      if (i + 1 < v.size())
        out += ", ";
    }
    out += "]";
    return out;
  }
};

template <typename T> struct StringMaker<std::expected<T, spymarine::error>> {
  static std::string convert(const std::expected<T, spymarine::error>& value) {
    if (value) {
      return StringMaker<T>::convert(*value);
    } else {
      return "Unexpected error: " + spymarine::error_message(value.error());
    }
  }
};

template <> struct StringMaker<spymarine::message_value> {
  static std::string convert(const spymarine::message_value& value) {
    return std::visit(
        spymarine::overloaded{
            [](const spymarine::invalid_value& invalid) {
              return std::format("invalid_value {{ id = {} }}", invalid.id());
            },
            [](const spymarine::numeric_value1& numeric) {
              const auto& bytes = numeric.raw_bytes();
              return std::format("numeric_value1 {{ id = {}, bytes = {} }}",
                                 numeric.id(),
                                 StringMaker<decltype(bytes)>::convert(bytes));
            },
            [](const spymarine::numeric_value3& numeric) {
              const auto& bytes = numeric.raw_bytes();
              return std::format("numeric_value3 {{ id = {}, bytes = {} }}",
                                 numeric.id(),
                                 StringMaker<decltype(bytes)>::convert(bytes));
            },
            [](const spymarine::string_value& str) {
              return std::format(R"(string_value {{ id = {}, str = "{}" }})",
                                 str.id(), str.str());
            },
        },
        value);
  }
};

template <> struct StringMaker<spymarine::firmware_version> {
  static std::string convert(const spymarine::firmware_version& fw) {
    return std::format("{}.{}", fw.major, fw.minor);
  }
};

template <> struct StringMaker<spymarine::system_info> {
  static std::string convert(const spymarine::system_info& info) {
    return std::format(
        "system_info {{ serial_number = {}, firmware_version = {} }}",
        info.serial_number,
        StringMaker<decltype(info.fw_version)>::convert(info.fw_version));
  }
};

template <> struct StringMaker<spymarine::count_info> {
  static std::string convert(const spymarine::count_info& info) {
    return std::format("count_info {{ device_count = {}, sensor_count = {} }}",
                       info.device_count, info.sensor_count);
  }
};

template <> struct StringMaker<spymarine::fluid_type> {
  static std::string convert(const spymarine::fluid_type& fluid) {
    switch (fluid) {
    case spymarine::fluid_type::fresh_water:
      return "fresh_water";
    case spymarine::fluid_type::fuel:
      return "fuel";
    case spymarine::fluid_type::waste_water:
      return "waste_water";
    case spymarine::fluid_type::unknown:
      return "unknown";
    }

    return "invalid_fluid_type";
  }
};

template <> struct StringMaker<spymarine::battery_type> {
  static std::string convert(const spymarine::battery_type& battery) {
    switch (battery) {
    case spymarine::battery_type::wet_low_maintenance:
      return "wet_low_maintenance";
    case spymarine::battery_type::wet_maintenance_free:
      return "wet_maintenance_free";
    case spymarine::battery_type::agm:
      return "agm";
    case spymarine::battery_type::deep_cycle:
      return "deep_cycle";
    case spymarine::battery_type::gel:
      return "gel";
    case spymarine::battery_type::lifepo4:
      return "lifepo4";
    case spymarine::battery_type::unknown:
      return "unknown";
    }

    return "invalid_battery_type";
  }
};

template <> struct StringMaker<spymarine::unit> {
  static std::string convert(spymarine::unit unit) {
    switch (unit) {
    case spymarine::unit::volts:
      return "V";
    case spymarine::unit::amps:
      return "A";
    case spymarine::unit::celsius:
      return "°C";
    case spymarine::unit::millibar:
      return "mbar";
    case spymarine::unit::millibars_per_hour:
      return "mbar/h";
    case spymarine::unit::ohms:
      return "Ω";
    case spymarine::unit::liters:
      return "L";
    case spymarine::unit::percent:
      return "%";
    case spymarine::unit::amp_hours:
      return "Ah";
    }

    return "invalid_unit";
  }
};

template <typename T, size_t Denominator>
struct StringMaker<spymarine::rational<T, Denominator>> {
  static std::string convert(const spymarine::rational<T, Denominator>& r) {
    return r.to_string();
  }
};

template <typename T, size_t Denominator, spymarine::unit Unit>
struct StringMaker<spymarine::device_property<T, Denominator, Unit>> {
  static std::string
  convert(const spymarine::device_property<T, Denominator, Unit>& r) {
    return std::format("{} {}",
                       StringMaker<decltype(r.value)>::convert(r.value),
                       StringMaker<spymarine::unit>::convert(r.unit));
  }
};

template <> struct StringMaker<spymarine::voltage_device2> {
  static std::string convert(const spymarine::voltage_device2& dev) {
    return std::format(
        "voltage_device {{ id = {}, name = {}, sensor_ids = {} }}", dev.id,
        StringMaker<decltype(dev.name)>::convert(dev.name),
        StringMaker<decltype(dev.sensor_ids)>::convert(dev.sensor_ids));
  }
};

template <> struct StringMaker<spymarine::resistive_device2> {
  static std::string convert(const spymarine::resistive_device2& dev) {
    return std::format(
        "resistive_device {{ id = {}, name = {}, sensor_ids = {} }}", dev.id,
        StringMaker<decltype(dev.name)>::convert(dev.name),
        StringMaker<decltype(dev.sensor_ids)>::convert(dev.sensor_ids));
  }
};

template <> struct StringMaker<spymarine::barometer_device2> {
  static std::string convert(const spymarine::barometer_device2& dev) {
    return std::format(
        "barometer_device {{ id = {}, name = {}, sensor_ids = {} }}", dev.id,
        StringMaker<decltype(dev.name)>::convert(dev.name),
        StringMaker<decltype(dev.sensor_ids)>::convert(dev.sensor_ids));
  }
};

template <> struct StringMaker<spymarine::current_device2> {
  static std::string convert(const spymarine::current_device2& dev) {
    return std::format(
        "current_device {{ id = {}, name = {}, sensor_ids = {} }}", dev.id,
        StringMaker<decltype(dev.name)>::convert(dev.name),
        StringMaker<decltype(dev.sensor_ids)>::convert(dev.sensor_ids));
  }
};

template <> struct StringMaker<spymarine::temperature_device2> {
  static std::string convert(const spymarine::temperature_device2& dev) {
    return std::format(
        "temperature_device {{ id = {}, name = {}, sensor_ids = {} }}", dev.id,
        StringMaker<decltype(dev.name)>::convert(dev.name),
        StringMaker<decltype(dev.sensor_ids)>::convert(dev.sensor_ids));
  }
};

template <> struct StringMaker<spymarine::battery_device2> {
  static std::string convert(const spymarine::battery_device2& dev) {
    return std::format(
        "battery_device {{ id = {}, name = {}, type = {}, "
        "capacity = {}, sensor_ids = {} }}",
        dev.id, StringMaker<decltype(dev.name)>::convert(dev.name),
        StringMaker<decltype(dev.type)>::convert(dev.type),
        StringMaker<decltype(dev.capacity)>::convert(dev.capacity),
        StringMaker<decltype(dev.sensor_ids)>::convert(dev.sensor_ids));
  }
};

template <> struct StringMaker<spymarine::tank_device2> {
  static std::string convert(const spymarine::tank_device2& dev) {
    return std::format(
        "tank_device_info {{ id = {}, name = {}, type = {}, capacity "
        "= {}, sensor_ids = {} }}",
        dev.id, StringMaker<decltype(dev.name)>::convert(dev.name),
        StringMaker<decltype(dev.type)>::convert(dev.type),
        StringMaker<decltype(dev.capacity)>::convert(dev.capacity),
        StringMaker<decltype(dev.sensor_ids)>::convert(dev.sensor_ids));
  }
};

template <> struct StringMaker<spymarine::unsupported_device2> {
  static std::string convert(const spymarine::unsupported_device2& dev) {
    return std::format(
        "unsupported_device {{ id = {}, type = {}, name = {}, sensor_ids "
        "= {} }}",
        dev.id, dev.raw_type,
        StringMaker<decltype(dev.name)>::convert(dev.name),
        StringMaker<decltype(dev.sensor_ids)>::convert(dev.sensor_ids));
  }
};

template <typename T, size_t Denominator, spymarine::unit Unit>
struct StringMaker<spymarine::sensor_value2<T, Denominator, Unit>> {
  static std::string
  convert(const spymarine::sensor_value2<T, Denominator, Unit>& r) {
    return std::format(
        "sensor_value {{ current = {} {}, average = {} {} }}",
        StringMaker<decltype(r.current_value)>::convert(r.current_value),
        StringMaker<spymarine::unit>::convert(r.unit), r.average_value,
        StringMaker<spymarine::unit>::convert(r.unit));
  }
};

template <> struct StringMaker<spymarine::voltage_sensor2> {
  static std::string convert(const spymarine::voltage_sensor2& s) {
    return std::format(
        "voltage_sensor {{ id = {}, parent_device_id = {}, value = {} }}", s.id,
        StringMaker<decltype(s.parent_device_id)>::convert(s.parent_device_id),
        StringMaker<decltype(s.value)>::convert(s.value));
  }
};

template <> struct StringMaker<spymarine::current_sensor2> {
  static std::string convert(const spymarine::current_sensor2& s) {
    return std::format(
        "current_sensor {{ id = {}, parent_device_id = {}, value = {} }}", s.id,
        StringMaker<decltype(s.parent_device_id)>::convert(s.parent_device_id),
        StringMaker<decltype(s.value)>::convert(s.value));
  }
};

template <> struct StringMaker<spymarine::temperature_sensor2> {
  static std::string convert(const spymarine::temperature_sensor2& s) {
    return std::format(
        "temperature_sensor {{ id = {}, parent_device_id = {}, value = {} }}",
        s.id,
        StringMaker<decltype(s.parent_device_id)>::convert(s.parent_device_id),
        StringMaker<decltype(s.value)>::convert(s.value));
  }
};

template <> struct StringMaker<spymarine::barometer_sensor2> {
  static std::string convert(const spymarine::barometer_sensor2& s) {
    return std::format(
        "barometer_sensor {{ id = {}, parent_device_id = {}, value = {} }}",
        s.id,
        StringMaker<decltype(s.parent_device_id)>::convert(s.parent_device_id),
        StringMaker<decltype(s.value)>::convert(s.value));
  }
};

template <> struct StringMaker<spymarine::resistive_sensor2> {
  static std::string convert(const spymarine::resistive_sensor2& s) {
    return std::format(
        "resistive_sensor {{ id = {}, parent_device_id = {}, value = {} }}",
        s.id,
        StringMaker<decltype(s.parent_device_id)>::convert(s.parent_device_id),
        StringMaker<decltype(s.value)>::convert(s.value));
  }
};

template <> struct StringMaker<spymarine::tank_sensor2> {
  static std::string convert(const spymarine::tank_sensor2& s) {
    return std::format(
        "tank_sensor {{ id = {}, parent_device_id = {}, volume = {}, level = "
        "{} }}",
        s.id,
        StringMaker<decltype(s.parent_device_id)>::convert(s.parent_device_id),
        StringMaker<decltype(s.volume)>::convert(s.volume),
        StringMaker<decltype(s.level)>::convert(s.level));
  }
};

template <> struct StringMaker<spymarine::battery_sensor2> {
  static std::string convert(const spymarine::battery_sensor2& s) {
    return std::format(
        "battery_charge_sensor {{ id = {}, parent_device_id = "
        "{}, charge = {}, remaining_capacity = {} }}",
        s.id,
        StringMaker<decltype(s.parent_device_id)>::convert(s.parent_device_id),
        StringMaker<decltype(s.charge)>::convert(s.charge),
        StringMaker<decltype(s.remaining_capacity)>::convert(
            s.remaining_capacity));
  }
};

template <> struct StringMaker<spymarine::unsupported_sensor2> {
  static std::string convert(const spymarine::unsupported_sensor2& s) {
    return std::format(
        "unsupported_sensor {{ id = {}, parent_device_id = {}, "
        "raw_type = {}, raw_value = {} }}",
        s.id,
        StringMaker<decltype(s.parent_device_id)>::convert(s.parent_device_id),
        s.raw_type, s.raw_value);
  }
};

} // namespace Catch
