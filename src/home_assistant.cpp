#include "spymarine/home_assistant.hpp"
#include "spymarine/device.hpp"
#include "spymarine/overloaded.hpp"

#include <format>
#include <optional>
#include <string>

namespace spymarine {
namespace {
std::string make_battery_discovery(const battery_device& battery) {
  constexpr auto format_string = R"(
{{
    "dev": {{
        "ids": "simarine_battery_{}",
        "name": "{}",
        "mf": "Simarine"
    }},
    "o": {{
        "name": "spymarine",
        "sw": "0.1",
        "url": "https://github.com/christopher-strack/spymarine-cpp"
    }},
    "cmps": {{
        "battery_{}": {{
            "p": "sensor",
            "device_class": "battery",
            "unit_of_measurement": "%",
            "value_template": "{{{{ value_json.battery }}}}",
            "unique_id": "battery_{}"
        }},
        "current_{}": {{
            "p": "sensor",
            "device_class": "current",
            "unit_of_measurement": "A",
            "value_template": "{{{{ value_json.current }}}}",
            "unique_id": "current_{}"
        }},
        "voltage_{}": {{
            "p": "sensor",
            "device_class": "voltage",
            "unit_of_measurement": "V",
            "value_template": "{{{{ value_json.voltage }}}}",
            "unique_id": "voltage_{}"
        }}
    }},
    "state_topic": "simarine_battery_{}/state",
    "qos": 1
}}
)";

  const auto device_id = battery.charge_sensor.state_index;
  const auto charge_id = battery.charge_sensor.state_index;
  const auto current_id = battery.current_sensor.state_index;
  const auto voltage_id = battery.voltage_sensor.state_index;

  return std::format(format_string, device_id, battery.name, charge_id,
                     charge_id, current_id, current_id, voltage_id, voltage_id,
                     device_id);
}

std::string make_tank_discovery(const tank_device& tank) {
  constexpr auto format_string = R"(
{{
    "dev": {{
        "ids": "simarine_tank_{}",
        "name": "{}",
        "mf": "Simarine"
    }},
    "o": {{
        "name": "spymarine",
        "sw": "0.1",
        "url": "https://github.com/christopher-strack/spymarine-cpp"
    }},
    "cmps": {{
        "volume_{}": {{
            "p": "sensor",
            "device_class": "volume",
            "unit_of_measurement": "L",
            "value_template": "{{{{ value_json.volume }}}}",
            "unique_id": "volume_{}"
        }},
        "level_{}": {{
            "p": "sensor",
            "unit_of_measurement": "%",
            "value_template": "{{{{ value_json.level }}}}",
            "unique_id": "level_{}"
        }}
    }},
    "state_topic": "simarine_tank_{}/state",
    "qos": 1
}}
)";

  const auto device_id = tank.volume_sensor.state_index;
  const auto volume_id = tank.volume_sensor.state_index;
  const auto level_id = tank.level_sensor.state_index;

  return std::format(format_string, device_id, tank.name, volume_id, volume_id,
                     level_id, level_id, device_id);
}

std::string make_sensor_discovery(const auto& device, const char* type,
                                  const char* device_class, const char* unit) {
  constexpr auto format_string = R"(
{{
    "dev": {{
        "ids": "simarine_{}_{}",
        "name": "{}",
        "mf": "Simarine"
    }},
    "o": {{
        "name": "spymarine",
        "sw": "0.1",
        "url": "https://github.com/christopher-strack/spymarine-cpp"
    }},
    "cmps": {{
        "{}_{}": {{
            "p": "sensor",
            "device_class": "{}",
            "unit_of_measurement": "{}",
            "value_template": "{{{{ value_json.value }}}}",
            "unique_id": "{}_{}"
        }}
    }},
    "state_topic": "simarine_{}_{}/state",
    "qos": 1
}}
)";

  const auto device_id = device.device_sensor.state_index;
  const auto sensor_id = device.device_sensor.state_index;

  return std::format(format_string, type, device_id, device.name, type,
                     device_id, device_class, unit, type, sensor_id, type,
                     device_id);
}

std::string make_home_assistant_device_discovery_payload(const device& device) {
  return std::visit(
      overloaded{
          [](const pico_internal_device& d) {
            return make_sensor_discovery(d, "pico_internal", "voltage", "V");
          },
          [](const voltage_device& d) {
            return make_sensor_discovery(d, "voltage", "voltage", "V");
          },
          [](const current_device& d) {
            return make_sensor_discovery(d, "current", "current", "A");
          },
          [](const temperature_device& d) {
            return make_sensor_discovery(d, "temperature", "temperature", "°C");
          },
          [](const barometer_device& d) {
            return make_sensor_discovery(d, "barometer", "atmospheric_pressure",
                                         "mbar");
          },
          [](const resistive_device& d) {
            return make_sensor_discovery(d, "resistive", "None", "ohm");
          },
          [](const battery_device& d) { return make_battery_discovery(d); },
          [](const tank_device& d) { return make_tank_discovery(d); },
      },
      device);
}

std::string make_home_assistant_device_discovery_topic(const device& device) {
  return std::format("homeassistant/device/simarine_{}_{}/config",
                     device_type(device), device_id(device));
}
} // namespace

mqtt_message make_home_assistant_device_discovery(const device& device) {
  return mqtt_message{
      .topic = make_home_assistant_device_discovery_topic(device),
      .payload = make_home_assistant_device_discovery_payload(device),
  };
}

std::optional<std::string>
make_home_assistant_sensor_topic(const device& device) {
  using R = std::optional<std::string>;
  return std::visit(overloaded{
                        [](const battery_device& d) -> R {
                          return std::format("simarine_battery_{}/state",
                                             d.charge_sensor.state_index);
                        },
                        [](const tank_device& d) -> R {
                          return std::format("simarine_tank_{}/state",
                                             d.volume_sensor.state_index);
                        },
                        [](const temperature_device& d) -> R {
                          return std::format("simarine_temperature_{}/state",
                                             d.device_sensor.state_index);
                        },
                        [](const voltage_device& d) -> R {
                          return std::format("simarine_voltage_{}/state",
                                             d.device_sensor.state_index);
                        },
                        [](const current_device& d) -> R {
                          return std::format("simarine_current_{}/state",
                                             d.device_sensor.state_index);
                        },
                        [](const auto& d) -> R { return std::nullopt; },
                    },
                    device);
}

namespace {

std::string make_battery_sensor_message(const battery_device& battery) {
  constexpr auto format_string =
      R"({{"battery": {},"current": {},"voltage": {}}})";

  return std::format(format_string, battery.charge_sensor.value,
                     battery.current_sensor.value,
                     battery.voltage_sensor.value);
}

std::string make_tank_sensor_message(const tank_device& tank) {
  constexpr auto format_string = R"({{"volume": {},"level": {}}}
)";

  return std::format(format_string, tank.volume_sensor.value,
                     tank.level_sensor.value);
}

std::string make_sensor_message(const auto& device) {
  constexpr auto format_string = R"({{"value":{}}})";
  return std::format(format_string, device.device_sensor.value);
}

} // namespace

std::optional<std::string>
make_home_assistant_sensor_message(const device& device) {
  using R = std::optional<std::string>;
  return std::visit(
      overloaded{
          [](const battery_device& d) -> R {
            return make_battery_sensor_message(d);
          },
          [](const tank_device& d) -> R { return make_tank_sensor_message(d); },
          [](const temperature_device& d) -> R {
            return make_sensor_message(d);
          },
          [](const voltage_device& d) -> R { return make_sensor_message(d); },
          [](const current_device& d) -> R { return make_sensor_message(d); },
          [](const auto& d) -> R { return std::nullopt; },
      },
      device);
}

} // namespace spymarine
