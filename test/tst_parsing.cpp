#include "raw_data.hpp"

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

TEST_CASE("parse_response") {
  const auto valid_response = std::array<uint8_t, 30>{
      0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 0x85, 0xde, 0xc3,
      0x46, 0x00, 0x11, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x1f,
      0xff, 0x02, 0x01, 0x00, 0x00, 0x00, 0x29, 0xff, 0x13, 0x14};

  SECTION("returns message for valid response") {
    const auto expected_data =
        std::array<uint8_t, 14>{0x01, 0x01, 0x00, 0x00, 0x00, 0x1f, 0xff,
                                0x02, 0x01, 0x00, 0x00, 0x00, 0x29, 0xff};
    const auto expected_message = message{
        message_type::device_count,
        expected_data,
    };
    CHECK(parse_response(valid_response) == expected_message);
  }

  SECTION("returns nullopt if length is invalid") {
    auto invalid_response = valid_response;
    invalid_response[12] += 1;
    CHECK(parse_response(invalid_response) == std::nullopt);
  }

  SECTION("returns nullopt if crc is invalid") {
    auto invalid_response = valid_response;
    invalid_response[invalid_response.size() - 1] += 1;
    CHECK(parse_response(invalid_response) == std::nullopt);
  }
}

TEST_CASE("crc") {
  const auto message = header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x02,
                                    0x04, 0x8C, 0x55, 0x4B, 0x00, 0x03, 0xFF};
  const auto span = std::span{message.begin() + 1, message.size() - 2};
  CHECK(crc(span) == 43200);
}

TEST_CASE("make_request") {
  std::array<uint8_t, 1024> buffer;
  const auto expected_message =
      std::array{0x0,  0x0,  0x0,  0x0, 0x0, 0xff, 0x2,  0x4,
                 0x8c, 0x55, 0x4b, 0x0, 0x3, 0xff, 0xa8, 0xc0};
  CHECK_THAT(make_request(message_type::device_count, {}, buffer),
             Catch::Matchers::RangeEquals(expected_message));
}

TEST_CASE("make_device_request") {
  std::array<uint8_t, 1024> buffer;
  const auto expected_message = std::array{
      0x0,  0x0,  0x0, 0x0,  0x0, 0xff, 0x41, 0x4,  0x8c, 0x55, 0x4b, 0x0,
      0x16, 0xff, 0x0, 0x1,  0x0, 0x0,  0x0,  0x2a, 0xff, 0x1,  0x3,  0x0,
      0x0,  0x0,  0x0, 0xff, 0x0, 0x0,  0x0,  0x0,  0xff, 0xb6, 0xa1};
  CHECK_THAT(make_device_request(42, buffer),
             Catch::Matchers::RangeEquals(expected_message));
}

TEST_CASE("parse_device") {
  std::vector<device> devices;

  for (const auto& response : device_info_responses) {
    const auto message = parse_response(response);
    REQUIRE(message);

    const auto device = parse_device(message->data);
    REQUIRE(device);
    devices.push_back(*device);
  }

  const auto expected_devices = std::vector<device>{
      unknown_device{},
      unknown_device{},
      unknown_device{},
      null_device{},
      null_device{},
      barometer_device{"Barometer"},
      pico_internal_device{},
      null_device{},
      null_device{},
      null_device{},
      voltage_device{"ST107 [5596] 1"},
      voltage_device{"ST107 [5596] 2"},
      voltage_device{"ST107 [5596] 3"},
      resistive_device{"ST107 [5596] 1"},
      resistive_device{"ST107 [5596] 2"},
      resistive_device{"ST107 [5596] 3"},
      resistive_device{"ST107 [5596] 4"},
      unknown_device{},
      current_device{"SC303 [5499]"},
      voltage_device{"SC303 [5499] 1"},
      voltage_device{"SC303 [5499] 2"},
      resistive_device{"SC303 [5499] 1"},
      resistive_device{"SC303 [5499] 2"},
      resistive_device{"SC303 [5499] 3"},
      battery_device{"Bulltron", battery_type::lifepo4, 300.0f},
      temperature_device{"Batterie"},
      tank_device{"Frischwasser", fluid_type::fresh_water, 100.0f},
      battery_device{"Starterbatterie", battery_type::agm, 100.0f},
      tank_device{"Abwasser", fluid_type::waste_water, 70.0f},
      temperature_device{"Innen"},
      temperature_device{"Au\xa3\x65n "},
      temperature_device{"Boiler"},
  };
  CHECK(devices == expected_devices);
}

} // namespace spymarine
