#include "spymarine/message_value.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("numeric_value") {
  static constexpr auto bytes =
      std::array<uint8_t, 7>{0x01, 0x02, 0x03, 0x04, 0xdd, 0xee, 0xff};
  constexpr numeric_value1 value{std::span<const uint8_t, 7>{bytes}};

  STATIC_CHECK(value.first() == 0x0102);
  STATIC_CHECK(value.second() == 0x0304);
  STATIC_CHECK(value.number() == 0x01020304);
}

} // namespace spymarine
