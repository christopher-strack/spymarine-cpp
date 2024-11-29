#include "spymarine/device.hpp"
#include "spymarine/home_assistant.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("make_home_assistant_device_discovery") {
  const auto message = make_home_assistant_device_discovery(
      temperature_device{"Test Temperature", 0, 21.4});

  CHECK(message.topic == "homeassistant/device/simarine_temperature_0/config");
  CHECK(message.payload == R"(
{
    "dev": {
        "ids": "simarine_temperature_0",
        "name": "Test Temperature",
        "mf": "Simarine"
    },
    "o": {
        "name": "spymarine",
        "sw": "0.1",
        "url": "https://github.com/christopher-strack/spymarine-cpp"
    },
    "cmps": {
        "temperature_0": {
            "p": "sensor",
            "device_class": "temperature",
            "unit_of_measurement": "°C",
            "value_template": "{{ value_json.value }}",
            "unique_id": "temperature_0"
        }
    },
    "state_topic": "simarine_temperature_0/state",
    "qos": 1
}
)");
}

} // namespace spymarine
