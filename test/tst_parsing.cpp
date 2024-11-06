#include "raw_data.hpp"

#include "spymarine/parsing.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/generators/catch_generators.hpp>

namespace spymarine {
namespace {

using header_bytes = std::array<uint8_t, header_length>;

} // namespace

TEST_CASE("parse_header") {
  SECTION("returns header for valid bytes") {
    const auto [bytes, expected_header] = GENERATE(
        std::make_tuple(header_bytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                                     0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff},
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

TEST_CASE("parse_property_dict") {
  const auto message = parse_response(device_info_responses[0]);
  REQUIRE(message);

  const auto property_dict = parse_property_dict(message->data);
  CHECK(property_dict.numbers.size() == 28);
  CHECK(property_dict.strings.size() == 2);
}

TEST_CASE("device_from_property_dict") {
  std::vector<device> devices;
  for (const auto& response : device_info_responses) {
    const auto message = parse_response(response);
    REQUIRE(message);

    const auto property_dict = parse_property_dict(message->data);
    const auto device = device_from_property_dict(property_dict);

    devices.push_back(device);
  }

  CHECK(devices == std::vector<device>{
                       device{device_type::unknown, ""},
                       device{device_type::unknown, ""},
                       device{device_type::unknown, "Sensor 1"},
                       device{device_type::null, ""},
                       device{device_type::null, ""},
                       device{device_type::barometer, "Barometer"},
                       device{device_type::picoInternal, "PICO INTERNAL"},
                       device{device_type::null, ""},
                       device{device_type::null, ""},
                       device{device_type::null, ""},
                       device{device_type::voltage, "ST107 [5596] 1"},
                       device{device_type::voltage, "ST107 [5596] 2"},
                       device{device_type::voltage, "ST107 [5596] 3"},
                       device{device_type::resistive, "ST107 [5596] 1"},
                       device{device_type::resistive, "ST107 [5596] 2"},
                       device{device_type::resistive, "ST107 [5596] 3"},
                       device{device_type::resistive, "ST107 [5596] 4"},
                       device{device_type::unknown, "ST107 [5596]"},
                       device{device_type::current, "SC303 [5499]"},
                       device{device_type::voltage, "SC303 [5499] 1"},
                       device{device_type::voltage, "SC303 [5499] 2"},
                       device{device_type::resistive, "SC303 [5499] 1"},
                       device{device_type::resistive, "SC303 [5499] 2"},
                       device{device_type::resistive, "SC303 [5499] 3"},
                       device{device_type::battery, "Bulltron"},
                       device{device_type::temperature, "Batterie"},
                       device{device_type::tank, "Frischwasser"},
                       device{device_type::battery, "Starterbatterie"},
                       device{device_type::tank, "Abwasser"},
                       device{device_type::temperature, "Innen"},
                       device{device_type::temperature, "Au\xa3\x65n "},
                       device{device_type::temperature, "Boiler"},
                   });
}

} // namespace spymarine
