#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "raw_data.hpp"

#include "spymarine/device_info.hpp"
#include "spymarine/device_ostream.hpp" // IWYU pragma: keep
#include "spymarine/parse_device_info.hpp"
#include "spymarine/parse_message.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("parse_device_info") {
  SECTION("unknown_device") {
    static constexpr auto msg =
        parse_message(raw_unknown_device_1_response).value();

    STATIC_CHECK(parse_device_info(msg.values()) ==
                 device_info{unknown_device_info{.id = 0}});
  }

  SECTION("null_device") {
    static constexpr auto msg =
        parse_message(raw_null_device_1_response).value();

    STATIC_CHECK(parse_device_info(msg.values()) ==
                 device_info{null_device_info{.id = 3}});
  }

  SECTION("barometer_device") {
    static constexpr auto msg =
        parse_message(raw_barometer_device_response).value();

    STATIC_CHECK(
        parse_device_info(msg.values()) ==
        device_info{barometer_device_info{.id = 5, .name = "Barometer"}});
  }

  SECTION("pico_internal_device") {
    static constexpr auto msg =
        parse_message(raw_pico_internal_device_response).value();

    STATIC_CHECK(parse_device_info(msg.values()) ==
                 device_info{pico_internal_device_info{.id = 6}});
  }

  SECTION("voltage_device") {
    static constexpr auto msg =
        parse_message(raw_st107_voltage_device_1_response).value();

    STATIC_CHECK(
        parse_device_info(msg.values()) ==
        device_info{voltage_device_info{.id = 10, .name = "ST107 [5596] 1"}});
  }

  SECTION("resistive_device") {
    static constexpr auto msg =
        parse_message(raw_st107_resistive_device_1_response).value();

    STATIC_CHECK(
        parse_device_info(msg.values()) ==
        device_info{resistive_device_info{.id = 13, .name = "ST107 [5596] 1"}});
  }

  SECTION("current_device") {
    static constexpr auto msg =
        parse_message(raw_sc303_current_device_response).value();

    STATIC_CHECK(
        parse_device_info(msg.values()) ==
        device_info{current_device_info{.id = 18, .name = "SC303 [5499]"}});
  }

  SECTION("battery_device") {
    static constexpr auto msg =
        parse_message(raw_battery_device_1_response).value();

    STATIC_CHECK(parse_device_info(msg.values()) ==
                 device_info{battery_device_info{
                     .id = 24,
                     .name = "Bulltron",
                     .type = battery_type::lifepo4,
                     .capacity = battery_capacity{300'00},
                 }});
  }

  SECTION("temperature_device") {
    static constexpr auto msg =
        parse_message(raw_temperature_device_1_response).value();

    STATIC_CHECK(
        parse_device_info(msg.values()) ==
        device_info{temperature_device_info{.id = 25, .name = "Batterie"}});
  }

  SECTION("tank_device") {
    static constexpr auto msg =
        parse_message(raw_tank_device_1_response).value();

    STATIC_CHECK(parse_device_info(msg.values()) ==
                 device_info{tank_device_info{
                     .id = 26,
                     .name = "Frischwasser",
                     .type = fluid_type::fresh_water,
                     .capacity = tank_capacity{100'0},
                 }});
  }
}

} // namespace spymarine
