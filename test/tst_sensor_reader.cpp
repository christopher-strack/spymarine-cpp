#include "data.hpp"
#include "raw_data/sensor_state.hpp"

#include "spymarine/buffer.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/sensor_reader.hpp"

#include <catch2/catch_all.hpp>

#include <expected>

namespace spymarine {
namespace {
class mock_udp_socket {
public:
  std::expected<std::span<const uint8_t>, error>
  receive([[maybe_unused]] std::span<uint8_t> buffer) {
    return std::span<const uint8_t>{raw_state_response};
  }
};

class failing_udp_socket {
public:
  std::expected<std::span<const uint8_t>, error>
  receive([[maybe_unused]] std::span<uint8_t> buffer) {
    return std::unexpected{std::errc::io_error};
  }
};
} // namespace

TEST_CASE("sensor_reader") {
  auto devices = make_parsed_devices();
  static_buffer buffer;

  SECTION("valid message updates devices") {
    sensor_reader<mock_udp_socket> reader{buffer, devices, mock_udp_socket{}};

    REQUIRE(reader.read_and_update());

    CHECK(devices == make_parsed_devices_with_values());
  }

  SECTION("fails if udp socket fails") {
    sensor_reader<failing_udp_socket> reader{buffer, devices,
                                             failing_udp_socket{}};

    const auto result = reader.read_and_update();

    REQUIRE_FALSE(result);
    CHECK(std::holds_alternative<std::errc>(result.error()));
  }
}

} // namespace spymarine
