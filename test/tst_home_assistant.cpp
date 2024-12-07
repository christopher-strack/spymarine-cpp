#include "spymarine/device.hpp"
#include "spymarine/home_assistant.hpp"

#include <catch2/catch_all.hpp>
#include <nlohmann/json.hpp>

namespace spymarine {
namespace {
auto origin_object() {
  return nlohmann::json::object({
      {"name", "spymarine"},
      {"sw", PROJECT_VERSION},
      {"url", "https://github.com/christopher-strack/spymarine-cpp"},
  });
}
} // namespace

TEST_CASE("make_home_assistant_device_discovery") {
  SECTION("pico_internal_device") {
    const auto device = pico_internal_device{0, 12.1};
    const auto discovery_message =
        make_home_assistant_device_discovery_message(device);

    CHECK(discovery_message.topic ==
          "homeassistant/device/simarine_pico_internal_0/config");

    const auto discovery_json =
        nlohmann::json::parse(discovery_message.payload);
    CHECK(discovery_json["dev"] == nlohmann::json::object({
                                       {"ids", "simarine_pico_internal_0"},
                                       {"name", "Pico internal"},
                                       {"mf", "Simarine"},
                                   }));
    CHECK(discovery_json["o"] == origin_object());
    CHECK(discovery_json["cmps"] ==
          nlohmann::json::object({
              {"pico_internal_0",
               nlohmann::json::object({
                   {"p", "sensor"},
                   {"device_class", "voltage"},
                   {"unit_of_measurement", "V"},
                   {"value_template", "{{ value_json.value }}"},
                   {"unique_id", "pico_internal_0"},
               })},
          }));
    CHECK(discovery_json["state_topic"] == "simarine_pico_internal_0/state");
    CHECK(discovery_json["qos"] == 1);

    const auto state_message = make_home_assistant_state_message(device);

    const auto state_json = nlohmann::json::parse(state_message.payload);
    CHECK(state_message.topic == discovery_json["state_topic"].get<std::string>());
    CHECK(state_json == nlohmann::json::object({{"value", 12.1}}));
  }

  SECTION("voltage_device") {
    const auto device = voltage_device{"Some Voltage", 3, 12.8};
    const auto discovery_message =
        make_home_assistant_device_discovery_message(device);

    CHECK(discovery_message.topic ==
          "homeassistant/device/simarine_voltage_3/config");

    const auto discovery_json =
        nlohmann::json::parse(discovery_message.payload);
    CHECK(discovery_json["dev"] == nlohmann::json::object({
                                       {"ids", "simarine_voltage_3"},
                                       {"name", "Some Voltage"},
                                       {"mf", "Simarine"},
                                   }));
    CHECK(discovery_json["o"] == origin_object());
    CHECK(discovery_json["cmps"] ==
          nlohmann::json::object({
              {"voltage_3", nlohmann::json::object({
                                {"p", "sensor"},
                                {"device_class", "voltage"},
                                {"unit_of_measurement", "V"},
                                {"value_template", "{{ value_json.value }}"},
                                {"unique_id", "voltage_3"},
                            })},
          }));
    CHECK(discovery_json["state_topic"] == "simarine_voltage_3/state");
    CHECK(discovery_json["qos"] == 1);

    const auto state_message = make_home_assistant_state_message(device);

    const auto state_json = nlohmann::json::parse(state_message.payload);
    CHECK(state_message.topic == discovery_json["state_topic"].get<std::string>());
    CHECK(state_json == nlohmann::json::object({{"value", 12.8}}));
  }

  SECTION("current_device") {
    const auto device = current_device{"Some Current", 28, 1.1};
    const auto discovery_message =
        make_home_assistant_device_discovery_message(device);

    CHECK(discovery_message.topic ==
          "homeassistant/device/simarine_current_28/config");

    const auto discovery_json =
        nlohmann::json::parse(discovery_message.payload);
    CHECK(discovery_json["dev"] == nlohmann::json::object({
                                       {"ids", "simarine_current_28"},
                                       {"name", "Some Current"},
                                       {"mf", "Simarine"},
                                   }));
    CHECK(discovery_json["o"] == origin_object());
    CHECK(discovery_json["cmps"] ==
          nlohmann::json::object({
              {"current_28", nlohmann::json::object({
                                 {"p", "sensor"},
                                 {"device_class", "current"},
                                 {"unit_of_measurement", "A"},
                                 {"value_template", "{{ value_json.value }}"},
                                 {"unique_id", "current_28"},
                             })},
          }));
    CHECK(discovery_json["state_topic"] == "simarine_current_28/state");
    CHECK(discovery_json["qos"] == 1);

    const auto state_message = make_home_assistant_state_message(device);

    const auto state_json = nlohmann::json::parse(state_message.payload);
    CHECK(state_message.topic == discovery_json["state_topic"].get<std::string>());
    CHECK(state_json == nlohmann::json::object({{"value", 1.1}}));
  }

  SECTION("temperature_device") {
    const auto device = temperature_device{"Test Temperature", 7, 21.4};
    const auto discovery_message =
        make_home_assistant_device_discovery_message(device);

    CHECK(discovery_message.topic ==
          "homeassistant/device/simarine_temperature_7/config");

    const auto discovery_json =
        nlohmann::json::parse(discovery_message.payload);
    CHECK(discovery_json["dev"] == nlohmann::json::object({
                                       {"ids", "simarine_temperature_7"},
                                       {"name", "Test Temperature"},
                                       {"mf", "Simarine"},
                                   }));
    CHECK(discovery_json["o"] == origin_object());
    CHECK(
        discovery_json["cmps"] ==
        nlohmann::json::object({
            {"temperature_7", nlohmann::json::object({
                                  {"p", "sensor"},
                                  {"device_class", "temperature"},
                                  {"unit_of_measurement", "°C"},
                                  {"value_template", "{{ value_json.value }}"},
                                  {"unique_id", "temperature_7"},
                              })},
        }));
    CHECK(discovery_json["state_topic"] == "simarine_temperature_7/state");
    CHECK(discovery_json["qos"] == 1);

    const auto state_message = make_home_assistant_state_message(device);

    const auto state_json = nlohmann::json::parse(state_message.payload);
    CHECK(state_message.topic == discovery_json["state_topic"].get<std::string>());
    CHECK(state_json == nlohmann::json::object({{"value", 21.4}}));
  }

  SECTION("barometer_device") {
    const auto device = barometer_device{"Barometer", 14, 1134.5};
    const auto discovery_message =
        make_home_assistant_device_discovery_message(device);

    CHECK(discovery_message.topic ==
          "homeassistant/device/simarine_barometer_14/config");

    const auto discovery_json =
        nlohmann::json::parse(discovery_message.payload);
    CHECK(discovery_json["dev"] == nlohmann::json::object({
                                       {"ids", "simarine_barometer_14"},
                                       {"name", "Barometer"},
                                       {"mf", "Simarine"},
                                   }));
    CHECK(discovery_json["o"] == origin_object());
    CHECK(discovery_json["cmps"] ==
          nlohmann::json::object({
              {"barometer_14", nlohmann::json::object({
                                   {"p", "sensor"},
                                   {"device_class", "atmospheric_pressure"},
                                   {"unit_of_measurement", "mbar"},
                                   {"value_template", "{{ value_json.value }}"},
                                   {"unique_id", "barometer_14"},
                               })},
          }));
    CHECK(discovery_json["state_topic"] == "simarine_barometer_14/state");
    CHECK(discovery_json["qos"] == 1);

    const auto state_message = make_home_assistant_state_message(device);

    const auto state_json = nlohmann::json::parse(state_message.payload);
    CHECK(state_message.topic == discovery_json["state_topic"].get<std::string>());
    CHECK(state_json == nlohmann::json::object({{"value", 1134.5}}));
  }

  SECTION("resistive_device") {
    const auto device = resistive_device{"Some Resistive Device", 17, 88.5};
    const auto discovery_message =
        make_home_assistant_device_discovery_message(device);

    CHECK(discovery_message.topic ==
          "homeassistant/device/simarine_resistive_17/config");

    const auto discovery_json =
        nlohmann::json::parse(discovery_message.payload);
    CHECK(discovery_json["dev"] == nlohmann::json::object({
                                       {"ids", "simarine_resistive_17"},
                                       {"name", "Some Resistive Device"},
                                       {"mf", "Simarine"},
                                   }));
    CHECK(discovery_json["o"] == origin_object());
    CHECK(discovery_json["cmps"] ==
          nlohmann::json::object({
              {"resistive_17", nlohmann::json::object({
                                   {"p", "sensor"},
                                   {"device_class", "None"},
                                   {"unit_of_measurement", "ohm"},
                                   {"value_template", "{{ value_json.value }}"},
                                   {"unique_id", "resistive_17"},
                               })},
          }));
    CHECK(discovery_json["state_topic"] == "simarine_resistive_17/state");
    CHECK(discovery_json["qos"] == 1);

    const auto state_message = make_home_assistant_state_message(device);

    const auto state_json = nlohmann::json::parse(state_message.payload);
    CHECK(state_message.topic == discovery_json["state_topic"].get<std::string>());
    CHECK(state_json == nlohmann::json::object({{"value", 88.5}}));
  }

  SECTION("tank_device") {
    const auto device =
        tank_device{"Some Tank", fluid_type::fresh_water, 100, 13, 34.5, 20.1};
    const auto discovery_message =
        make_home_assistant_device_discovery_message(device);

    CHECK(discovery_message.topic ==
          "homeassistant/device/simarine_tank_13/config");

    const auto discovery_json =
        nlohmann::json::parse(discovery_message.payload);
    CHECK(discovery_json["dev"] == nlohmann::json::object({
                                       {"ids", "simarine_tank_13"},
                                       {"name", "Some Tank"},
                                       {"mf", "Simarine"},
                                   }));
    CHECK(discovery_json["o"] == origin_object());
    CHECK(discovery_json["cmps"] ==
          nlohmann::json::object({
              {"level_13", nlohmann::json::object({
                               {"p", "sensor"},
                               {"unit_of_measurement", "%"},
                               {"value_template", "{{ value_json.level }}"},
                               {"unique_id", "level_13"},
                           })},
              {"volume_13", nlohmann::json::object({
                                {"p", "sensor"},
                                {"device_class", "volume"},
                                {"unit_of_measurement", "L"},
                                {"value_template", "{{ value_json.volume }}"},
                                {"unique_id", "volume_13"},
                            })},
          }));
    CHECK(discovery_json["state_topic"] == "simarine_tank_13/state");
    CHECK(discovery_json["qos"] == 1);

    const auto state_message = make_home_assistant_state_message(device);

    const auto state_json = nlohmann::json::parse(state_message.payload);
    CHECK(state_message.topic == discovery_json["state_topic"].get<std::string>());
    CHECK(state_json ==
          nlohmann::json::object({{"volume", 34.5}, {"level", 20.1}}));
  }

  SECTION("battery_device") {
    const auto device = battery_device{
        "Some Battery", battery_type::lifepo4, 320, 8, 89.3, 293.1, 5.7, 13.1};
    const auto discovery_message =
        make_home_assistant_device_discovery_message(device);

    CHECK(discovery_message.topic ==
          "homeassistant/device/simarine_battery_8/config");

    const auto discovery_json =
        nlohmann::json::parse(discovery_message.payload);
    CHECK(discovery_json["dev"] == nlohmann::json::object({
                                       {"ids", "simarine_battery_8"},
                                       {"name", "Some Battery"},
                                       {"mf", "Simarine"},
                                   }));
    CHECK(discovery_json["o"] == origin_object());
    CHECK(discovery_json["cmps"] ==
          nlohmann::json::object({
              {"battery_8", nlohmann::json::object({
                                {"p", "sensor"},
                                {"device_class", "battery"},
                                {"unit_of_measurement", "%"},
                                {"value_template", "{{ value_json.battery }}"},
                                {"unique_id", "battery_8"},
                            })},
              {"current_9", nlohmann::json::object({
                                {"p", "sensor"},
                                {"device_class", "current"},
                                {"unit_of_measurement", "A"},
                                {"value_template", "{{ value_json.current }}"},
                                {"unique_id", "current_9"},
                            })},
              {"voltage_10", nlohmann::json::object({
                                 {"p", "sensor"},
                                 {"device_class", "voltage"},
                                 {"unit_of_measurement", "V"},
                                 {"value_template", "{{ value_json.voltage }}"},
                                 {"unique_id", "voltage_10"},
                             })},
          }));
    CHECK(discovery_json["state_topic"] == "simarine_battery_8/state");
    CHECK(discovery_json["qos"] == 1);

    const auto state_message = make_home_assistant_state_message(device);

    const auto state_json = nlohmann::json::parse(state_message.payload);
    CHECK(state_message.topic == discovery_json["state_topic"].get<std::string>());
    CHECK(state_json == nlohmann::json::object({
                            {"battery", 89.3},
                            {"current", 5.7},
                            {"voltage", 13.1},
                        }));
  }
}

} // namespace spymarine
