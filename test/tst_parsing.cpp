#include "raw_data.hpp"

#include "spymarine/device.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/parsing.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>
#include <cstdint>

namespace spymarine {
namespace {

using header_bytes = std::array<uint8_t, header_size>;

} // namespace

TEST_CASE("parse_header") {
  SECTION("returns header for valid bytes") {
    const auto [bytes, expected_header] = GENERATE(
        std::make_tuple(header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                                     0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff},
                        header{0x41, 353}),
        std::make_tuple(header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                                     0x86, 0xa3, 0xc5, 0x41, 0x01, 0x61, 0xff},
                        header{0x41, 353}),
        std::make_tuple(header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb0,
                                     0x85, 0xde, 0xc3, 0x46, 0x01, 0x14, 0xff},
                        header{0xb0, 276}),
        std::make_tuple(header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02,
                                     0x85, 0xde, 0xc3, 0x46, 0x00, 0x11, 0xff},
                        header{0x02, 17}));

    CHECK(parse_header(bytes) == expected_header);
  }

  SECTION("returns nullopt if not enough bytes were provided") {
    constexpr auto valid_bytes =
        header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                     0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff};
    CHECK(parse_header(std::span{valid_bytes}.subspan(0, 13)) == std::nullopt);
  }

  SECTION("returns nullopt if header doesn't match pattern") {
    auto bytes = header_bytes{0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                              0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff};
    CHECK(parse_header(bytes) == std::nullopt);
  }
}

TEST_CASE("parse_message") {
  const auto valid_message = std::array<uint8_t, 30>{
      0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 0x85, 0xde, 0xc3,
      0x46, 0x00, 0x11, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x1f,
      0xff, 0x02, 0x01, 0x00, 0x00, 0x00, 0x29, 0xff, 0x13, 0x14};

  SECTION("returns message for valid message") {
    const auto expected_data =
        std::array<uint8_t, 14>{0x01, 0x01, 0x00, 0x00, 0x00, 0x1f, 0xff,
                                0x02, 0x01, 0x00, 0x00, 0x00, 0x29, 0xff};
    const auto expected_message = message{
        message_type::device_count,
        expected_data,
    };
    CHECK(parse_message(valid_message) == expected_message);
  }

  SECTION("returns nullopt if length is invalid") {
    auto invalid_message = valid_message;
    invalid_message[12] += 1;
    CHECK(parse_message(invalid_message) == std::nullopt);
  }

  SECTION("returns nullopt if crc is invalid") {
    auto invalid_message = valid_message;
    invalid_message[invalid_message.size() - 1] += 1;
    CHECK(parse_message(invalid_message) == std::nullopt);
  }
}

TEST_CASE("crc") {
  const auto data = header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x02,
                                 0x04, 0x8C, 0x55, 0x4B, 0x00, 0x03, 0xFF};
  const auto span = std::span{data.begin() + 1, data.size() - 2};
  CHECK(crc(span) == 43200);
}

TEST_CASE("write_message_data") {
  std::array<uint8_t, 1024> buffer;
  const auto expected_data =
      std::array{0x0,  0x0,  0x0,  0x0, 0x0, 0xff, 0x2,  0x4,
                 0x8c, 0x55, 0x4b, 0x0, 0x3, 0xff, 0xa8, 0xc0};
  CHECK_THAT(write_message_data({message_type::device_count}, buffer),
             Catch::Matchers::RangeEquals(expected_data));
}

TEST_CASE("write_device_info_data") {
  std::array<uint8_t, 19> buffer;

  const auto expected_message =
      message{message_type::device_info,
              std::array<uint8_t, 19>{0x00, 0x01, 0x00, 0x00, 0x00, 0x2a, 0xff,
                                      0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0xff,
                                      0x00, 0x00, 0x00, 0x00, 0xff}};

  CHECK(make_device_info_message(42, buffer) == expected_message);
}

} // namespace spymarine
