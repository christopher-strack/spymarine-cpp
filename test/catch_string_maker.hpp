#pragma once

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

} // namespace Catch
