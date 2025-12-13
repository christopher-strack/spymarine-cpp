#include "data.hpp"

#include "spymarine/home_assistant.hpp"

#include <catch2/catch_all.hpp>
#include <nlohmann/json.hpp>

namespace spymarine {

using namespace nlohmann;

TEST_CASE("make_home_assistant_device_discovery2") {
  const auto config = home_assistant_state_config{};
  const auto devices = make_parsed_devices2_with_sensors();

  SECTION("voltage_device") {
    const auto dev = devices[6];
    const auto discovery = make_home_assistant_device_discovery(
        dev, parsed_sensors2_with_value, parsed_system_info);

    const auto expected_discovery_json = json::object({
        {
            "dev",
            {
                {"ids", "simarine.2245968710.volt6"},
                {"name", "PICO INTERNAL"},
                {"mf", "Simarine"},
                {"sn", "2245968710"},
                {"sw", "1.17"},
            },
        },
        {
            "o",
            {
                {"name", "spymarine"},
                {"sw", SPYMARINE_PROJECT_VERSION},
                {"url", "https://github.com/christopher-strack/spymarine-cpp"},
            },
        },
        {
            "cmps",
            {
                {
                    "volt5",
                    {
                        {"p", "sensor"},
                        {"dev_cla", "voltage"},
                        {"unit_of_meas", "V"},
                        {"val_tpl", "{{ value_json.volt }}"},
                        {"uniq_id", "simarine.2245968710.volt6.volt5"},
                    },
                },
            },
        },
        {"stat_t", "simarine/2245968710/volt6/state"},
        {"qos", 1},
    });
    CHECK(json::parse(to_json(discovery)) == expected_discovery_json);

    const auto states = make_home_assistant_device_sensor_states(
        dev, parsed_sensors2_with_value, config);

    const auto expected_states_json = json::object({{"volt", 13.26}});
    CHECK(json::parse(to_json_object(states)) == expected_states_json);
  }

  SECTION("barometer_device") {
    const auto dev = devices[5];
    const auto discovery = make_home_assistant_device_discovery(
        dev, parsed_sensors2_with_value, parsed_system_info);

    const auto expected_discovery_json = json::object({
        {
            "dev",
            {
                {"ids", "simarine.2245968710.baro5"},
                {"name", "Barometer"},
                {"mf", "Simarine"},
                {"sn", "2245968710"},
                {"sw", "1.17"},
            },
        },
        {
            "o",
            {
                {"name", "spymarine"},
                {"sw", SPYMARINE_PROJECT_VERSION},
                {"url", "https://github.com/christopher-strack/spymarine-cpp"},
            },
        },
        {
            "cmps",
            {
                {
                    "baro3",
                    {
                        {"p", "sensor"},
                        {"dev_cla", "atmospheric_pressure"},
                        {"unit_of_meas", "mbar"},
                        {"val_tpl", "{{ value_json.baro }}"},
                        {"uniq_id", "simarine.2245968710.baro5.baro3"},
                    },
                },
            },
        },
        {"stat_t", "simarine/2245968710/baro5/state"},
        {"qos", 1},
    });
    CHECK(json::parse(to_json(discovery)) == expected_discovery_json);

    const auto states = make_home_assistant_device_sensor_states(
        dev, parsed_sensors2_with_value, config);

    const auto expected_states_json = json::object({{"baro", 979.83}});
    CHECK(json::parse(to_json_object(states)) == expected_states_json);
  }

  SECTION("resistive_device") {
    const auto dev = devices[13];
    const auto discovery = make_home_assistant_device_discovery(
        dev, parsed_sensors2_with_value, parsed_system_info);

    const auto expected_discovery_json = json::object({
        {
            "dev",
            {
                {"ids", "simarine.2245968710.res13"},
                {"name", "ST107 [5596] 1"},
                {"mf", "Simarine"},
                {"sn", "2245968710"},
                {"sw", "1.17"},
            },
        },
        {
            "o",
            {
                {"name", "spymarine"},
                {"sw", SPYMARINE_PROJECT_VERSION},
                {"url", "https://github.com/christopher-strack/spymarine-cpp"},
            },
        },
        {
            "cmps",
            {
                {
                    "res14",
                    {
                        {"p", "sensor"},
                        {"unit_of_meas", "Ω"},
                        {"name", "Resistive Sensor"},
                        {"val_tpl", "{{ value_json.res }}"},
                        {"uniq_id", "simarine.2245968710.res13.res14"},
                    },
                },
            },
        },
        {"stat_t", "simarine/2245968710/res13/state"},
        {"qos", 1},
    });
    CHECK(json::parse(to_json(discovery)) == expected_discovery_json);

    const auto states = make_home_assistant_device_sensor_states(
        dev, parsed_sensors2_with_value, config);

    const auto expected_states_json = json::object({{"res", 19121}});
    CHECK(json::parse(to_json_object(states)) == expected_states_json);
  }

  SECTION("current_device") {
    const auto dev = devices[18];
    const auto discovery = make_home_assistant_device_discovery(
        dev, parsed_sensors2_with_value, parsed_system_info);

    const auto expected_discovery_json = json::object({
        {
            "dev",
            {
                {"ids", "simarine.2245968710.cur18"},
                {"name", "SC303 [5499]"},
                {"mf", "Simarine"},
                {"sn", "2245968710"},
                {"sw", "1.17"},
            },
        },
        {
            "o",
            {
                {"name", "spymarine"},
                {"sw", SPYMARINE_PROJECT_VERSION},
                {"url", "https://github.com/christopher-strack/spymarine-cpp"},
            },
        },
        {
            "cmps",
            {
                {
                    "cur19",
                    {
                        {"p", "sensor"},
                        {"dev_cla", "current"},
                        {"unit_of_meas", "A"},
                        {"val_tpl", "{{ value_json.cur }}"},
                        {"uniq_id", "simarine.2245968710.cur18.cur19"},
                    },
                },
            },
        },
        {"stat_t", "simarine/2245968710/cur18/state"},
        {"qos", 1},
    });
    CHECK(json::parse(to_json(discovery)) == expected_discovery_json);

    const auto states = make_home_assistant_device_sensor_states(
        dev, parsed_sensors2_with_value, config);

    const auto expected_states_json = json::object({{"cur", -1.23}});
    CHECK(json::parse(to_json_object(states)) == expected_states_json);
  }

  SECTION("temperature_device") {
    const auto dev = devices[29];
    const auto discovery = make_home_assistant_device_discovery(
        dev, parsed_sensors2_with_value, parsed_system_info);

    const auto expected_discovery_json = json::object({
        {
            "dev",
            {
                {"ids", "simarine.2245968710.temp29"},
                {"name", "Innen"},
                {"mf", "Simarine"},
                {"sn", "2245968710"},
                {"sw", "1.17"},
            },
        },
        {
            "o",
            {
                {"name", "spymarine"},
                {"sw", SPYMARINE_PROJECT_VERSION},
                {"url", "https://github.com/christopher-strack/spymarine-cpp"},
            },
        },
        {
            "cmps",
            {
                {
                    "temp39",
                    {
                        {"p", "sensor"},
                        {"dev_cla", "temperature"},
                        {"unit_of_meas", "°C"},
                        {"val_tpl", "{{ value_json.temp }}"},
                        {"uniq_id", "simarine.2245968710.temp29.temp39"},
                    },
                },
            },
        },
        {"stat_t", "simarine/2245968710/temp29/state"},
        {"qos", 1},
    });
    CHECK(json::parse(to_json(discovery)) == expected_discovery_json);

    const auto states = make_home_assistant_device_sensor_states(
        dev, parsed_sensors2_with_value, config);

    const auto expected_states_json = json::object({{"temp", 10.7}});
    CHECK(json::parse(to_json_object(states)) == expected_states_json);
  }

  SECTION("battery_device") {
    const auto dev = devices[24];
    const auto discovery = make_home_assistant_device_discovery(
        dev, parsed_sensors2_with_value, parsed_system_info);

    const auto expected_discovery_json = json::object({
        {
            "dev",
            {
                {"ids", "simarine.2245968710.batt24"},
                {"name", "Bulltron"},
                {"mf", "Simarine"},
                {"sn", "2245968710"},
                {"sw", "1.17"},
            },
        },
        {
            "o",
            {
                {"name", "spymarine"},
                {"sw", SPYMARINE_PROJECT_VERSION},
                {"url", "https://github.com/christopher-strack/spymarine-cpp"},
            },
        },
        {
            "cmps",
            {
                {
                    "batt26",
                    {
                        {"p", "sensor"},
                        {"dev_cla", "battery"},
                        {"unit_of_meas", "%"},
                        {"val_tpl", "{{ value_json.batt }}"},
                        {"uniq_id", "simarine.2245968710.batt24.batt26"},
                    },
                },
                {
                    "cap26",
                    {
                        {"p", "sensor"},
                        {"name", "Remaining Capacity"},
                        {"unit_of_meas", "Ah"},
                        {"val_tpl", "{{ value_json.cap }}"},
                        {"uniq_id", "simarine.2245968710.batt24.cap26"},
                    },
                },
                {
                    "cur27",
                    {
                        {"p", "sensor"},
                        {"dev_cla", "current"},
                        {"unit_of_meas", "A"},
                        {"val_tpl", "{{ value_json.cur }}"},
                        {"uniq_id", "simarine.2245968710.batt24.cur27"},
                    },
                },
                {
                    "volt28",
                    {
                        {"p", "sensor"},
                        {"dev_cla", "voltage"},
                        {"unit_of_meas", "V"},
                        {"val_tpl", "{{ value_json.volt }}"},
                        {"uniq_id", "simarine.2245968710.batt24.volt28"},
                    },
                },
            },
        },
        {"stat_t", "simarine/2245968710/batt24/state"},
        {"qos", 1},
    });
    CHECK(json::parse(to_json(discovery)) == expected_discovery_json);

    const auto states = make_home_assistant_device_sensor_states(
        dev, parsed_sensors2_with_value, config);

    const auto expected_states_json = json::object({
        {"batt", 87.9},
        {"cap", 263.7},
        {"cur", -1.23},
        {"volt", 13.314},
    });
    CHECK(json::parse(to_json_object(states)) == expected_states_json);
  }

  SECTION("tank_device") {
    const auto dev = devices[28];
    const auto discovery = make_home_assistant_device_discovery(
        dev, parsed_sensors2_with_value, parsed_system_info);

    const auto expected_discovery_json = json::object({
        {
            "dev",
            {
                {"ids", "simarine.2245968710.tank28"},
                {"name", "Abwasser"},
                {"mf", "Simarine"},
                {"sn", "2245968710"},
                {"sw", "1.17"},
            },
        },
        {
            "o",
            {
                {"name", "spymarine"},
                {"sw", SPYMARINE_PROJECT_VERSION},
                {"url", "https://github.com/christopher-strack/spymarine-cpp"},
            },
        },
        {
            "cmps",
            {
                {
                    "vol38",
                    {
                        {"p", "sensor"},
                        {"dev_cla", "volume_storage"},
                        {"unit_of_meas", "L"},
                        {"val_tpl", "{{ value_json.vol }}"},
                        {"uniq_id", "simarine.2245968710.tank28.vol38"},
                    },
                },
                {
                    "lvl38",
                    {
                        {"p", "sensor"},
                        {"name", "Tank Level"},
                        {"unit_of_meas", "%"},
                        {"val_tpl", "{{ value_json.lvl }}"},
                        {"uniq_id", "simarine.2245968710.tank28.lvl38"},
                    },
                },
            },
        },
        {"stat_t", "simarine/2245968710/tank28/state"},
        {"qos", 1},
    });
    CHECK(json::parse(to_json(discovery)) == expected_discovery_json);

    const auto states = make_home_assistant_device_sensor_states(
        dev, parsed_sensors2_with_value, config);

    const auto expected_states_json = json::object({
        {"lvl", 5.2},
        {"vol", 3.7},
    });
    CHECK(json::parse(to_json_object(states)) == expected_states_json);
  }

  SECTION("unsupported_device") {
    const auto dev = devices[1];
    const auto discovery = make_home_assistant_device_discovery(
        dev, parsed_sensors2_with_value, parsed_system_info);

    const auto expected_discovery_json = json::object({
        {
            "dev",
            {
                {"ids", "simarine.2245968710.unsupp1"},
                {"name", json::value_t::null},
                {"mf", "Simarine"},
                {"sn", "2245968710"},
                {"sw", "1.17"},
            },
        },
        {
            "o",
            {
                {"name", "spymarine"},
                {"sw", SPYMARINE_PROJECT_VERSION},
                {"url", "https://github.com/christopher-strack/spymarine-cpp"},
            },
        },
        {"cmps", json::object()},
        {"stat_t", "simarine/2245968710/unsupp1/state"},
        {"qos", 1},
    });
    CHECK(json::parse(to_json(discovery)) == expected_discovery_json);

    const auto states = make_home_assistant_device_sensor_states(
        dev, parsed_sensors2_with_value, config);
    CHECK(json::parse(to_json_object(states)) == json::object());
  }
}

} // namespace spymarine
