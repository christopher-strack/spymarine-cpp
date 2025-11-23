#pragma once

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

} // namespace Catch
