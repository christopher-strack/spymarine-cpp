#include "data.hpp"
#include "raw_data.hpp"

#include "spymarine/device_ostream.hpp"
#include "spymarine/sensor_reader.hpp"

#include <catch2/catch_all.hpp>

#include <expected>

namespace spymarine {
namespace {
class mock_udp_socket {
public:
  std::expected<std::span<const uint8_t>, std::error_code>
  receive(std::span<uint8_t> buffer) {
    return std::span<const uint8_t>{raw_state_response};
  }
};
} // namespace

TEST_CASE("sensor_reader") {
  auto devices = parsed_devices;
  sensor_reader<mock_udp_socket> reader{devices, mock_udp_socket{}};

  REQUIRE(reader.read_and_update());

  CHECK(devices == parsed_devices_with_values);
}

} // namespace spymarine
