#include "spymarine/message_value.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("numeric_value") {
  static constexpr auto bytes =
      std::to_array<uint8_t>({0x01, 0x01, 0x01, 0x02, 0x03, 0x04});
  constexpr numeric_value1 value{std::span<const uint8_t, 6>{bytes}};

  STATIC_CHECK(value.low_int16() == 0x0102);
  STATIC_CHECK(value.high_int16() == 0x0304);
  STATIC_CHECK(value.int32() == 0x01020304);
}

} // namespace spymarine
