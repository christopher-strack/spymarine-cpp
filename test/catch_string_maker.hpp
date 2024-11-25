#pragma once

#include "spymarine/error.hpp"

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
} // namespace Catch
