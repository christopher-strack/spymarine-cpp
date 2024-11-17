#include "data.hpp"
#include "raw_data.hpp"

#include "spymarine/device.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/parse_message.hpp"

#include <catch2/catch_all.hpp>

#include <expected>

namespace spymarine {

TEST_CASE("device") {
  SECTION("update_sensor_states") {
    const auto state_message = parse_message(raw_state_response);
    REQUIRE(state_message);
    REQUIRE(state_message->type == message_type::sensor_state);

    std::vector<device> devices = parsed_devices;
    auto sensor_map = build_sensor_map(devices);

    update_sensor_states(*state_message, sensor_map);

    CHECK(devices == parsed_devices_with_values);
  }
}

} // namespace spymarine
