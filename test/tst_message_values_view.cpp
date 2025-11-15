#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/overloaded.hpp"

#include <catch2/catch_all.hpp>

#include <format>
#include <ranges>
#include <string>

namespace spymarine {

using resolved_value = std::variant<int32_t, std::string, invalid_value>;

} // namespace spymarine

namespace Catch {

using namespace spymarine;
using namespace std::string_literals;

template <> struct StringMaker<resolved_value> {
  static std::string convert(const resolved_value& value) {
    return std::visit(
        overloaded{
            [](const int32_t& n) { return std::to_string(n); },
            [](const std::string& str) { return std::format(R"("{}")", str); },
            [](const invalid_value&) { return "invalid_value{}"s; }},
        value);
  }
};

} // namespace Catch

namespace spymarine {

constexpr resolved_value to_resolved_value(message_value value) {
  return std::visit(
      overloaded{
          [](const numeric_value& nv) -> resolved_value { return nv.number(); },
          [](const string_value& sv) -> resolved_value { return sv.str(); },
          [](const invalid_value& iv) -> resolved_value { return iv; }},
      value);
}

constexpr auto resolved_values_view(const std::span<const uint8_t>& bytes) {
  return message_values_view{bytes} | std::views::transform([](auto&& mv) {
           return std::tuple{mv.id, to_resolved_value(mv.value)};
         }) |
         std::ranges::to<std::vector>();
}

using resolved_values =
    decltype(resolved_values_view(std::declval<std::span<const uint8_t>>()));

TEST_CASE("message_values_view") {
  SECTION("empty buffer") {
    static constexpr auto empty_buffer = std::span<const uint8_t>{};
    constexpr auto values = message_values_view{empty_buffer};

    STATIC_REQUIRE(values.begin() == values.end());
  }

  SECTION("buffer too small for header") {
    // Buffer with only 1 byte (need at least 2 for id + type)
    static constexpr auto raw_data = std::to_array<uint8_t>({0x01});

    STATIC_REQUIRE(resolved_values_view(raw_data) ==
                   resolved_values{{0, invalid_value{}}});
  }

  SECTION("type 1 - insufficient payload") {
    SECTION("valid payload") {
      // ID + type + exactly 4 bytes
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x01, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{1, 0x12345678}});
    }

    SECTION("valid payload with multiple values") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({// first value
                                  0x05, 0x01, 0x12, 0x34, 0x56, 0x78, 0xff,
                                  // second value
                                  0x06, 0x01, 0x23, 0x45, 0x67, 0x89, 0xff});

      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{5, 0x12345678}, {6, 0x23456789}});
    }

    SECTION("no payload") {
      // ID + type but no payload data
      static constexpr auto raw_data = std::to_array<uint8_t>({0x01, 0x01});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{1, invalid_value{}}});
    }

    SECTION("partial payload") {
      // ID + type + only 2 bytes (need 4)
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x01, 0x12, 0x34});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{1, invalid_value{}}});
    }
  }

  SECTION("type 3 - insufficient payload") {
    SECTION("valid payload") {
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{2, 0x12345678}});
    }

    SECTION("valid payload with multiple values") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({// first value
                                  0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x12, 0x34, 0x56, 0x78, 0xff,
                                  // second value
                                  0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x23, 0x45, 0x67, 0x89, 0xff});

      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{1, 0x12345678}, {2, 0x23456789}});
    }

    SECTION("no payload") {
      static constexpr auto raw_data = std::to_array<uint8_t>({0x02, 0x03});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{2, invalid_value{}}});
    }

    SECTION("partial payload - 5 bytes") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{2, invalid_value{}}});
    }

    SECTION("partial payload - 8 bytes") {
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{2, invalid_value{}}});
    }
  }

  SECTION("type 4 - string values corner cases") {
    SECTION("valid string") {
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x69, 0x00});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{3, "Hi"}});
    }

    SECTION("valid string followed by valid message value") {
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {// String "Hi" (10 bytes)
           0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x69, 0x00, 0xff,
           // type 1 value
           0x05, 0x01, 0x00, 0x00, 0x12, 0x34, 0xff});

      auto result = resolved_values_view(raw_data);
      REQUIRE(result == resolved_values{{3, "Hi"}, {5, 0x1234}});
    }

    SECTION("no payload") {
      static constexpr auto raw_data = std::to_array<uint8_t>({0x03, 0x04});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{3, invalid_value{}}});
    }

    SECTION("insufficient initial payload") {
      // Need at least 5 bytes before string data
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x03, 0x04, 0x00, 0x00});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{3, invalid_value{}}});
    }

    SECTION("no null terminator") {
      // String without null terminator
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x48, 0x65, 0x6c, 0x6c, 0x6f});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{3, invalid_value{}}});
    }

    SECTION("empty string") {
      // Null terminator immediately after header
      static constexpr auto raw_data = std::to_array<uint8_t>(
          {0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{3, ""}});
    }
  }

  SECTION("invalid types") {
    SECTION("type 0") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x00, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{1, invalid_value{}}});
    }

    SECTION("type 2") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x02, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{1, invalid_value{}}});
    }

    SECTION("type 5") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0x05, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{1, invalid_value{}}});
    }

    SECTION("type 255") {
      static constexpr auto raw_data =
          std::to_array<uint8_t>({0x01, 0xFF, 0x12, 0x34, 0x56, 0x78});
      STATIC_REQUIRE(resolved_values_view(raw_data) ==
                     resolved_values{{1, invalid_value{}}});
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

    STATIC_REQUIRE(resolved_values_view(raw_data) == resolved_values{
                                                         {1, 66},
                                                         {2, invalid_value{}},
                                                     });
  }
}

} // namespace spymarine
