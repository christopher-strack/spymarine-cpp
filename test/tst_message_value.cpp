#include "spymarine/message_value.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("numeric_value1") {
  SECTION("valid numeric_value1") {
    static constexpr auto bytes =
        std::to_array<uint8_t>({0x08, 0x01, 0x01, 0x02, 0x03, 0x04});
    constexpr numeric_value1 value{std::span{bytes}};

    STATIC_CHECK(value.id() == 0x08);
    STATIC_CHECK(value.low_int16() == 0x0102);
    STATIC_CHECK(value.high_int16() == 0x0304);
    STATIC_CHECK(value.int32() == 0x01020304);
  }

  SECTION("from_bytes") {
    static constexpr auto bytes =
        std::to_array<uint8_t>({0x08, 0x01, 0x01, 0x02, 0x03, 0x04, 0xff});
    constexpr auto value = numeric_value1::from_bytes(std::span{bytes});

    STATIC_CHECK(value.id() == 0x08);
    STATIC_CHECK(value.int32() == 0x01020304);
  }
}

TEST_CASE("numeric_value2") {
  SECTION("valid numeric_value3") {
    static constexpr auto bytes = std::to_array<uint8_t>(
        {0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78});
    constexpr numeric_value3 value{std::span{bytes}};

    STATIC_CHECK(value.id() == 0x02);
    STATIC_CHECK(value.low_int16() == 0x1234);
    STATIC_CHECK(value.high_int16() == 0x5678);
    STATIC_CHECK(value.int32() == 0x12345678);
  }

  SECTION("from_bytes") {
    static constexpr auto bytes =
        std::to_array<uint8_t>({0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12,
                                0x34, 0x56, 0x78, 0xff});
    constexpr auto value = numeric_value3::from_bytes(std::span{bytes});

    STATIC_CHECK(value.id() == 0x02);
    STATIC_CHECK(value.int32() == 0x12345678);
  }
}

TEST_CASE("string_value") {
  SECTION("valid string") {
    static constexpr auto bytes = std::to_array<uint8_t>(
        {0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x69});
    constexpr string_value value{std::span{bytes}};

    STATIC_CHECK(value.id() == 0x03);
    STATIC_CHECK(value.str() == "Hi");
  }

  SECTION("valid empty") {
    static constexpr auto bytes =
        std::to_array<uint8_t>({0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00});
    constexpr string_value value{std::span{bytes}};

    STATIC_CHECK(value.id() == 0x05);
    STATIC_CHECK(value.str() == "");
  }
  SECTION("from_bytes") {
    static constexpr auto bytes = std::to_array<uint8_t>(
        {0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x69, 0x00, 0xff});
    constexpr auto value = string_value::from_bytes(std::span{bytes});

    STATIC_REQUIRE(value.has_value());
    STATIC_CHECK(value->id() == 0x03);
    STATIC_CHECK(value->str() == "Hi");
  }
}

} // namespace spymarine
