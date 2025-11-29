#pragma once

#include "spymarine/device2.hpp"
#include "spymarine/device_info.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/overloaded.hpp"

#include <catch2/catch_all.hpp>

#include <expected>
#include <format>

namespace Catch {

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

template <> struct StringMaker<spymarine::device_info> {
  static std::string convert(const spymarine::device_info& di) {
    return std::visit(
        spymarine::overloaded{
            [](const spymarine::pico_internal_device_info& info) {
              return std::format("pico_internal_device_info {{ id = {} }}",
                                 info.id);
            },
            [](const spymarine::voltage_device_info& info) {
              return std::format(
                  "voltage_device_info {{ id = {}, name = \"{}\" }}", info.id,
                  info.name);
            },
            [](const spymarine::resistive_device_info& info) {
              return std::format(
                  "resistive_device_info {{ id = {}, name = \"{}\" }}", info.id,
                  info.name);
            },
            [](const spymarine::barometer_device_info& info) {
              return std::format(
                  "barometer_device_info {{ id = {}, name = \"{}\" }}", info.id,
                  info.name);
            },
            [](const spymarine::current_device_info& info) {
              return std::format(
                  "current_device_info {{ id = {}, name = \"{}\" }}", info.id,
                  info.name);
            },
            [](const spymarine::temperature_device_info& info) {
              return std::format(
                  "temperature_device_info {{ id = {}, name = \"{}\" }}",
                  info.id, info.name);
            },
            [](const spymarine::battery_device_info& info) {
              return std::format(
                  "battery_device_info {{ id = {}, name = "
                  "\"{}\", type = {}, capacity = {} }}",
                  info.id, info.name,
                  StringMaker<spymarine::battery_type>::convert(info.type),
                  info.capacity.to_float());
            },
            [](const spymarine::tank_device_info& info) {
              return std::format(
                  "tank_device_info {{ id = {}, name = "
                  "\"{}\", type = {}, capacity = {} }}",
                  info.id, info.name,
                  StringMaker<spymarine::fluid_type>::convert(info.type),
                  info.capacity.to_float());
            },
            [](const spymarine::null_device_info& info) {
              return std::format("null_device_info {{ id = {} }}", info.id);
            },
            [](const spymarine::unknown_device_info& info) {
              return std::format("unknown_device_info {{ id = {} }}", info.id);
            },
        },
        di);
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

template <typename T, size_t Denominator, spymarine::unit Unit>
struct StringMaker<spymarine::rational<T, Denominator, Unit>> {
  static std::string
  convert(const spymarine::rational<T, Denominator, Unit>& r) {
    return std::format("{:.{}f} {}", r.to_float(),
                       static_cast<int>(std::log10(Denominator)),
                       StringMaker<spymarine::unit>::convert(r.unit));
  }
};

template <> struct StringMaker<spymarine::tank_values> {
  static std::string convert(const spymarine::tank_values& values) {
    return std::format(
        "tank_values {{ volume = {}, level = {} }}",
        StringMaker<spymarine::tank_volume>::convert(values.volume),
        StringMaker<spymarine::tank_level>::convert(values.level));
  }
};

template <> struct StringMaker<spymarine::battery_charge_values> {
  static std::string convert(const spymarine::battery_charge_values& values) {
    return std::format(
        "battery_charge_values {{ charge = {}, remaining_capacity = {} }}",
        StringMaker<spymarine::battery_charge>::convert(values.charge),
        StringMaker<spymarine::battery_capacity>::convert(
            values.remaining_capacity));
  }
};

template <typename T> struct StringMaker<spymarine::sensor2<T>> {
  static std::string convert(const spymarine::sensor2<T>& sen) {
    return std::format(
        "sensor {{ id = {}, value = {} }}", sen.id(),
        StringMaker<decltype(sen.value())>::convert(sen.value()));
  }
};

template <> struct StringMaker<spymarine::device2> {
  static std::string convert(const spymarine::device2& di) {
    return std::visit(
        spymarine::overloaded{
            [](const spymarine::pico_internal_device2& device) {
              return std::format(
                  "pico_internal_device2 {{ info = {} }}\n",
                  StringMaker<spymarine::device_info>::convert(device.info));
            },
            [](const spymarine::voltage_device2& device) {
              return std::format(
                  "voltage_device2 {{ info = {}, sensor = {} }}\n",
                  StringMaker<spymarine::device_info>::convert(device.info),
                  StringMaker<decltype(device.sensor)>::convert(device.sensor));
            },
            [](const spymarine::current_device2& device) {
              return std::format(
                  "current_device2 {{ info = {}, sensor = {} }}\n",
                  StringMaker<spymarine::device_info>::convert(device.info),
                  StringMaker<decltype(device.sensor)>::convert(device.sensor));
            },
            [](const spymarine::temperature_device2& device) {
              return std::format(
                  "temperature_device2 {{ info = {}, sensor = {} }}\n",
                  StringMaker<spymarine::device_info>::convert(device.info),
                  StringMaker<decltype(device.sensor)>::convert(device.sensor));
            },
            [](const spymarine::barometer_device2& device) {
              return std::format(
                  "barometer_device2 {{ info = {}, sensor = {} }}\n",
                  StringMaker<spymarine::device_info>::convert(device.info),
                  StringMaker<decltype(device.sensor)>::convert(device.sensor));
            },
            [](const spymarine::resistive_device2& device) {
              return std::format(
                  "resistive_device2 {{ info = {}, sensor = {} }}\n",
                  StringMaker<spymarine::device_info>::convert(device.info),
                  StringMaker<decltype(device.sensor)>::convert(device.sensor));
            },
            [](const spymarine::tank_device2& device) {
              return std::format(
                  "tank_device2 {{ info = {}, sensor = {} }}",
                  StringMaker<spymarine::device_info>::convert(device.info),
                  StringMaker<decltype(device.sensor)>::convert(device.sensor));
            },
            [](const spymarine::battery_device2& device) {
              return std::format(
                  "battery_device2 {{ info = {}, charge_sensor = {}, "
                  "current_sensor = {}, voltage_sensor = {} }}\n",
                  StringMaker<spymarine::device_info>::convert(device.info),
                  StringMaker<decltype(device.charge_sensor)>::convert(
                      device.charge_sensor),
                  StringMaker<decltype(device.current_sensor)>::convert(
                      device.current_sensor),
                  StringMaker<decltype(device.voltage_sensor)>::convert(
                      device.voltage_sensor));
            }},
        di);
  }
};

} // namespace Catch
