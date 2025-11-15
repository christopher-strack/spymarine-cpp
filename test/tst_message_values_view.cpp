#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"

#include <catch2/catch_all.hpp>

#include <ranges>
#include <string>

namespace Catch {

using namespace spymarine;
using namespace std::string_literals;

template <> struct StringMaker<message_value> {
  static std::string convert(const message_value& value) {
    return std::visit(
        [](const auto& v) {
          const auto bytes = v.raw_bytes();
          return StringMaker<decltype(bytes)>::convert(bytes);
        },
        value);
  }
};

} // namespace Catch

namespace spymarine {

constexpr auto message_values(const std::span<const uint8_t>& bytes) {
  return message_values_view{bytes} | std::ranges::to<std::vector>();
}

TEST_CASE("message_values_view") {
  SECTION("empty buffer") {
    static constexpr auto empty_buffer = std::span<const uint8_t>{};
    constexpr auto values = message_values_view{empty_buffer};

    STATIC_REQUIRE(values.begin() == values.end());
  }

  SECTION("buffer too small for header") {
    // Buffer with only 1 byte (need at least 2 for id + type)
    static constexpr auto raw_data = std::to_array<uint8_t>({0x01});

    STATIC_REQUIRE(message_values(raw_data) == std::vector<message_value>{});
  }

  SECTION("type 1 - insufficient payload") {
    SECTION("valid payload") {
      // ID + type + exactly 4 bytes
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x01, 0x12, 0x34, 0x56, 0x78});

      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{numeric_value1{raw_data}});
    }

    SECTION("valid payload with multiple values") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({// first value
                                  0x05, 0x01, 0x12, 0x34, 0x56, 0x78, 0xff,
                                  // second value
                                  0x06, 0x01, 0x23, 0x45, 0x67, 0x89, 0xff});

      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{
                         numeric_value1{std::span{raw_data}.subspan<0, 6>()},
                         numeric_value1{std::span{raw_data}.subspan<7, 6>()},
                     });
    }

    SECTION("no payload") {
      // ID + type but no payload data
      static constexpr auto raw_data = std::to_array<uint8_t>({0x01, 0x01});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{1}});
    }

    SECTION("partial payload") {
      // ID + type + only 2 bytes (need 4)
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x01, 0x12, 0x34});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{1}});
    }
  }

  SECTION("type 3 - insufficient payload") {
    SECTION("valid payload") {
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{numeric_value3{raw_data}});
    }

    SECTION("valid payload with multiple values") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({// first value
                                  0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x12, 0x34, 0x56, 0x78, 0xff,
                                  // second value
                                  0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x23, 0x45, 0x67, 0x89, 0xff});

      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{
                         numeric_value3{std::span{raw_data}.subspan<0, 11>()},
                         numeric_value3{std::span{raw_data}.subspan<12, 11>()},
                     });
    }

    SECTION("no payload") {
      static constexpr auto raw_data = std::to_array<uint8_t>({0x02, 0x03});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{2}});
    }

    SECTION("partial payload - 5 bytes") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{2}});
    }

    SECTION("partial payload - 8 bytes") {
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{2}});
    }
  }

  SECTION("type 4 - string values corner cases") {
    SECTION("valid string") {
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x69, 0x00});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{
                         string_value{std::span{raw_data}.subspan(0, 9)},
                     });
    }

    SECTION("valid string followed by valid message value") {
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {// String "Hi" (10 bytes)
           0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x69, 0x00, 0xff,
           // type 1 value
           0x05, 0x01, 0x00, 0x00, 0x12, 0x34, 0xff});

      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{
                         string_value{std::span{raw_data}.subspan(0, 9)},
                         numeric_value1{std::span{raw_data}.subspan<11, 6>()},
                     });
    }

    SECTION("no payload") {
      static constexpr auto raw_data = std::to_array<uint8_t>({0x03, 0x04});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{3}});
    }

    SECTION("insufficient initial payload") {
      // Need at least 5 bytes before string data
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x03, 0x04, 0x00, 0x00});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{3}});
    }

    SECTION("no null terminator") {
      // String without null terminator
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x48, 0x65, 0x6c, 0x6c, 0x6f});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{3}});
    }

    SECTION("empty string") {
      // Null terminator immediately after header
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{
                         string_value{std::span{raw_data}.subspan(0, 7)},
                     });
    }
  }

  SECTION("invalid types") {
    SECTION("type 0") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x00, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{1}});
    }

    SECTION("type 2") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x02, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{1}});
    }

    SECTION("type 5") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x05, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{1}});
    }

    SECTION("type 255") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0xFF, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(message_values(raw_data) ==
                     std::vector<message_value>{invalid_value{1}});
    }
  }

  SECTION("advancement stops on invalid value") {
    static constexpr auto raw_data =
        std::to_array<uint8_t>({// Valid type 1
                                0x01, 0x01, 0x00, 0x00, 0x00, 0x42, 0xff,
                                // Invalid type (type 2)
                                0x02, 0x02, 0x11, 0x22, 0x33, 0x44, 0xff,
                                // Valid type 4 (string)
                                0x03, 0x01, 0x00, 0x00, 0x00, 0x12});

    STATIC_REQUIRE(message_values(raw_data) ==
                   std::vector<message_value>{
                       numeric_value1{std::span{raw_data}.subspan<0, 6>()},
                       invalid_value{2},
                   });
  }
}

} // namespace spymarine
