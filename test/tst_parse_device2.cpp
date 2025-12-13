#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "raw_data/devices.hpp"

#include "spymarine/device.hpp"
#include "spymarine/parse_device2.hpp"
#include "spymarine/parse_message.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("parse_device2") {
  SECTION("barometer_device") {
    static constexpr auto msg =
        parse_message(raw_barometer_device_response).value();

    CHECK(parse_device2(msg.values()) ==
          device2{barometer_device2{.id = 5, .name = "Barometer"}});
  }

  SECTION("voltage_device") {
    static constexpr auto msg =
        parse_message(raw_st107_voltage_device_1_response).value();

    CHECK(parse_device2(msg.values()) ==
          device2{voltage_device2{.id = 10, .name = "ST107 [5596] 1"}});
  }

  SECTION("resistive_device") {
    static constexpr auto msg =
        parse_message(raw_st107_resistive_device_1_response).value();

    CHECK(parse_device2(msg.values()) ==
          device2{resistive_device2{.id = 13, .name = "ST107 [5596] 1"}});
  }

  SECTION("current_device") {
    static constexpr auto msg =
        parse_message(raw_sc303_current_device_response).value();

    CHECK(parse_device2(msg.values()) ==
          device2{current_device2{.id = 18, .name = "SC303 [5499]"}});
  }

  SECTION("battery_device") {
    static constexpr auto msg =
        parse_message(raw_battery_device_1_response).value();

    CHECK(parse_device2(msg.values()) ==
          device2{battery_device2{
              .id = 24,
              .name = "Bulltron",
              .type = battery_type::lifepo4,
              .capacity = battery_capacity{300'00},
          }});
  }

  SECTION("temperature_device") {
    static constexpr auto msg =
        parse_message(raw_temperature_device_1_response).value();

    CHECK(parse_device2(msg.values()) ==
          device2{temperature_device2{.id = 25, .name = "Batterie"}});
  }

  SECTION("tank_device") {
    static constexpr auto msg =
        parse_message(raw_tank_device_1_response).value();

    CHECK(parse_device2(msg.values()) == device2{tank_device2{
                                             .id = 26,
                                             .name = "Frischwasser",
                                             .type = fluid_type::fresh_water,
                                             .capacity = tank_capacity{100'0},
                                         }});
  }

  SECTION("unsupported_device_info") {
    static constexpr auto msg =
        parse_message(raw_unknown_device_1_response).value();

    CHECK(parse_device2(msg.values()) ==
          device2{unsupported_device2{
              .id = 0, .raw_type = 10, .name = std::nullopt}});
  }
}

} // namespace spymarine
