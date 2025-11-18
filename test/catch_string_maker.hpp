#pragma once

#include "spymarine/error.hpp"
#include "spymarine/message_value.hpp"

#include <catch2/catch_all.hpp>

#include <expected>

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
        [](const auto& v) {
          const auto bytes = v.raw_bytes();
          return StringMaker<decltype(bytes)>::convert(bytes);
        },
        value);
  }
};

} // namespace Catch
