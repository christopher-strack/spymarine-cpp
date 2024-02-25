#include "rawData.hpp"

#include "spymarine/parsing.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/generators/catch_generators.hpp>

namespace spymarine {
namespace {

using HeaderBytes = std::array<uint8_t, headerLength>;

} // namespace

TEST_CASE("parseHeader") {
  SECTION("returns Header for valid bytes") {
    const auto [bytes, expectedHeader] = GENERATE(
        std::make_tuple(HeaderBytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                                    0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff},
                        Header{0x41, 353}),
        std::make_tuple(HeaderBytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xb0,
                                    0x85, 0xde, 0xc3, 0x46, 0x01, 0x14, 0xff},
                        Header{0xb0, 276}),
        std::make_tuple(HeaderBytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02,
                                    0x85, 0xde, 0xc3, 0x46, 0x00, 0x11, 0xff},
                        Header{0x02, 17}));

    CHECK(parseHeader(bytes) == expectedHeader);
  }

  SECTION("returns nullopt if not enough bytes were provided") {
    constexpr auto validBytes =
        HeaderBytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                    0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff};
    CHECK(parseHeader(std::span{validBytes}.subspan(0, 13)) == std::nullopt);
  }

  SECTION("returns nullopt if header doesn't match pattern") {
    auto bytes = HeaderBytes{0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x41,
                             0x85, 0xde, 0xc3, 0x46, 0x01, 0x61, 0xff};
    CHECK(parseHeader(bytes) == std::nullopt);
  }
}

TEST_CASE("parseResponse") {
  const auto validResponse = std::array<uint8_t, 30>{
      0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 0x85, 0xde, 0xc3,
      0x46, 0x00, 0x11, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x1f,
      0xff, 0x02, 0x01, 0x00, 0x00, 0x00, 0x29, 0xff, 0x13, 0x14};

  SECTION("returns message for valid response") {
    const auto expectedData =
        std::array<uint8_t, 14>{0x01, 0x01, 0x00, 0x00, 0x00, 0x1f, 0xff,
                                0x02, 0x01, 0x00, 0x00, 0x00, 0x29, 0xff};
    const auto expectedMessage = Message{
        MessageType::deviceCount,
        expectedData,
    };
    CHECK(parseResponse(validResponse) == expectedMessage);
  }

  SECTION("returns nullopt if length is invalid") {
    auto invalidResponse = validResponse;
    invalidResponse[12] += 1;
    CHECK(parseResponse(invalidResponse) == std::nullopt);
  }

  SECTION("returns nullopt if crc is invalid") {
    auto invalidResponse = validResponse;
    invalidResponse[invalidResponse.size() - 1] += 1;
    CHECK(parseResponse(invalidResponse) == std::nullopt);
  }
}

TEST_CASE("crc") {
  const auto message = HeaderBytes{0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x02,
                                   0x04, 0x8C, 0x55, 0x4B, 0x00, 0x03, 0xFF};
  const auto span = std::span{message.begin() + 1, message.size() - 2};
  CHECK(crc(span) == 43200);
}

TEST_CASE("parseValueMap") {
  const auto message = parseResponse(deviceInfoResponse);
  REQUIRE(message);

  const auto valueMap = parseValueMap(message->data);
  CHECK(valueMap.numbers.size() == 28);
  CHECK(valueMap.strings.size() == 2);
}

} // namespace spymarine
