#include "spymarine/parse_message.hpp"

#include <catch2/catch_all.hpp>
#include <cstdint>

namespace spymarine {
namespace {

using header_bytes = std::array<uint8_t, header_size>;

} // namespace

TEST_CASE("parse_header") {
  SECTION("returns header for valid bytes") {
    SECTION("header 1") {
      constexpr auto bytes =
          header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                       0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff};

      STATIC_CHECK(parse_header(bytes) == header{0x41, 353});
    }

    SECTION("header 2") {
      constexpr auto bytes =
          header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                       0x86, 0xa3, 0xc5, 0x41, 0x01, 0x61, 0xff};

      STATIC_CHECK(parse_header(bytes) == header{0x41, 353});
    }

    SECTION("header 3") {
      constexpr auto bytes =
          header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb0,
                       0x85, 0xde, 0xc3, 0x46, 0x01, 0x14, 0xff};

      STATIC_CHECK(parse_header(bytes) == header{0xb0, 276});
    }

    SECTION("header 4") {
      constexpr auto bytes =
          header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02,
                       0x85, 0xde, 0xc3, 0x46, 0x00, 0x11, 0xff};

      STATIC_CHECK(parse_header(bytes) == header{0x02, 17});
    }
  }

  SECTION("returns error if not enough bytes were provided") {
    constexpr auto valid_bytes =
        header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                     0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff};

    STATIC_CHECK(parse_header(std::span{valid_bytes}.subspan(0, 13)).error() ==
                 error{parse_error::invalid_data_length});
  }

  SECTION("returns error if header doesn't match pattern") {
    constexpr auto bytes =
        header_bytes{0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                     0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff};

    STATIC_CHECK(parse_header(bytes).error() ==
                 error{parse_error::invalid_header});
  }
}

TEST_CASE("parse_message") {
  static constexpr auto valid_message = std::to_array<uint8_t>(
      {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 0x85, 0xde, 0xc3,
       0x46, 0x00, 0x11, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x1f,
       0xff, 0x02, 0x01, 0x00, 0x00, 0x00, 0x29, 0xff, 0x13, 0x14});

  SECTION("returns message for valid message") {
    static constexpr auto expected_data =
        std::to_array<uint8_t>({0x01, 0x01, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x02,
                                0x01, 0x00, 0x00, 0x00, 0x29, 0xff});
    constexpr auto expected_message = message{
        message_type::device_count,
        expected_data,
    };
    STATIC_CHECK(parse_message(valid_message) == expected_message);
  }

  SECTION("returns error if length is invalid") {
    constexpr auto invalid_message = []() {
      auto msg = valid_message;
      msg[12] += 1;
      return msg;
    }();

    STATIC_CHECK(parse_message(invalid_message).error() ==
                 error{parse_error::invalid_data_length});
  }

  SECTION("returns error if crc is invalid") {
    constexpr auto invalid_message = []() {
      auto msg = valid_message;
      msg[msg.size() - 1] += 1;
      return msg;
    }();

    STATIC_CHECK(parse_message(invalid_message).error() ==
                 error{parse_error::invalid_crc});
  }
}

TEST_CASE("crc") {
  static constexpr auto bytes =
      header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x02,
                   0x04, 0x8C, 0x55, 0x4B, 0x00, 0x03, 0xFF};
  constexpr auto span = std::span{bytes.begin() + 1, bytes.size() - 2};
  STATIC_CHECK(crc(span) == 43200);
}

} // namespace spymarine
