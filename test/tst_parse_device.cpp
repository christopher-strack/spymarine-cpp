#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "raw_data/devices.hpp"

#include "spymarine/device.hpp"
#include "spymarine/device_ostream.hpp" // IWYU pragma: keep
#include "spymarine/parse_device.hpp"
#include "spymarine/parse_message.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("parse_device") {
  SECTION("unknown_device") {
    static constexpr auto msg =
        parse_message(raw_unknown_device_1_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{unknown_device{}});
  }

  SECTION("null_device") {
    static constexpr auto msg =
        parse_message(raw_null_device_1_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) == parsed_device{null_device{}});
  }

  SECTION("barometer_device") {
    static constexpr auto msg =
        parse_message(raw_barometer_device_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{barometer_device{"Barometer", 0}});
  }

  SECTION("pico_internal_device") {
    static constexpr auto msg =
        parse_message(raw_pico_internal_device_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{pico_internal_device{0}});
  }

  SECTION("voltage_device") {
    static constexpr auto msg =
        parse_message(raw_st107_voltage_device_1_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{voltage_device{"ST107 [5596] 1", 0}});
  }

  SECTION("resistive_device") {
    static constexpr auto msg =
        parse_message(raw_st107_resistive_device_1_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{resistive_device{"ST107 [5596] 1", 0}});
  }

  SECTION("current_device") {
    static constexpr auto msg =
        parse_message(raw_sc303_current_device_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{current_device{"SC303 [5499]", 0}});
  }

  SECTION("battery_device") {
    static constexpr auto msg =
        parse_message(raw_battery_device_1_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{battery_device{"Bulltron", battery_type::lifepo4,
                                              300, 0}});
  }

  SECTION("temperature_device") {
    static constexpr auto msg =
        parse_message(raw_temperature_device_1_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{temperature_device{"Batterie", 0}});
  }

  SECTION("tank_device") {
    static constexpr auto msg =
        parse_message(raw_tank_device_1_response).value();

    STATIC_CHECK(parse_device(msg.values(), 0) ==
                 parsed_device{tank_device{"Frischwasser",
                                           fluid_type::fresh_water, 100, 0}});
  }
}

} // namespace spymarine
