#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string_view>
#include <variant>

namespace spymarine {

class numeric_value {
public:
  explicit numeric_value(std::span<const uint8_t, 4> bytes);

  int16_t first() const;
  int16_t second() const;
  int32_t number() const;

private:
  std::array<uint8_t, 4> _bytes;
};

struct invalid_value {};

using message_value =
    std::variant<numeric_value, std::string_view, invalid_value>;

} // namespace spymarine
