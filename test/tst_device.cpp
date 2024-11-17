#include "raw_data.hpp"

#include "spymarine/device.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/parse_message.hpp"

#include <catch2/catch_all.hpp>

#include <expected>

namespace spymarine {
namespace {

const auto expected_devices = std::vector<device>{
    barometer_device{"Barometer", 3},
    pico_internal_device{5},
    voltage_device{"ST107 [5596] 1", 11},
    voltage_device{"ST107 [5596] 2", 12},
    voltage_device{"ST107 [5596] 3", 13},
    resistive_device{"ST107 [5596] 1", 14},
    resistive_device{"ST107 [5596] 2", 15},
    resistive_device{"ST107 [5596] 3", 16},
    resistive_device{"ST107 [5596] 4", 17},
    current_device{"SC303 [5499]", 19},
    voltage_device{"SC303 [5499] 1", 21},
    voltage_device{"SC303 [5499] 2", 22},
    resistive_device{"SC303 [5499] 1", 23},
    resistive_device{"SC303 [5499] 2", 24},
    resistive_device{"SC303 [5499] 3", 25},
    battery_device{"Bulltron", battery_type::lifepo4, 300.0f, 26},
    temperature_device{"Batterie", 31},
    tank_device{"Frischwasser", fluid_type::fresh_water, 100.0f, 32},
    battery_device{"Starterbatterie", battery_type::agm, 100.0f, 33},
    tank_device{"Abwasser", fluid_type::waste_water, 70.0f, 38},
    temperature_device{"Innen", 39},
    temperature_device{"Au\xa3\x65n ", 40},
    temperature_device{"Boiler", 41},
};

} // namespace

TEST_CASE("device") {
  SECTION("update_sensor_states") {
    const auto state_message = parse_message(raw_state_response);
    REQUIRE(state_message);
    REQUIRE(state_message->type == message_type::sensor_state);

    std::vector<device> devices = expected_devices;
    auto sensor_map = build_sensor_map(devices);

    update_sensor_states(*state_message, sensor_map);

    CHECK(devices == expected_devices);
  }
}

} // namespace spymarine
