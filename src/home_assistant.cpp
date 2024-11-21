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
        "name": "{}"
    }},
    "o": {{
        "name": "spymarine",
        "sw": "0.1",
        "url": "https://github.com/christopher-strack/esp-simarine-home-assistant"
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
        }},
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
        "name": "{}"
    }},
    "o": {{
        "name": "spymarine",
        "sw": "0.1",
        "url": "https://github.com/christopher-strack/esp-simarine-home-assistant"
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
        }},
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
                                  const char* unit) {
  constexpr auto format_string = R"(
{{
    "dev": {{
        "ids": "simarine_{}_{}",
        "name": "{}"
    }},
    "o": {{
        "name": "spymarine",
        "sw": "0.1",
        "url": "https://github.com/christopher-strack/esp-simarine-home-assistant"
    }},
    "cmps": {{
        "{}_{}": {{
            "p": "sensor",
            "device_class": "{}",
            "unit_of_measurement": "{}",
            "value_template": "{{{{ value_json.value }}}}",
            "unique_id": "{}_{}"
        }},
    }},
    "state_topic": "simarine_{}_{}/state",
    "qos": 1
}}
)";

  const auto device_id = device.device_sensor.state_index;
  const auto sensor_id = device.device_sensor.state_index;

  return std::format(format_string, type, device_id, device.name, type,
                     device_id, type, unit, type, sensor_id, type, device_id);
}

std::string make_temperature_discovery(const temperature_device& temperature) {
  return make_sensor_discovery(temperature, "temperature", "°C");
}

std::string make_voltage_discovery(const voltage_device& voltage) {
  return make_sensor_discovery(voltage, "voltage", "V");
}

std::string make_current_discovery(const current_device& current) {
  return make_sensor_discovery(current, "current", "A");
}
} // namespace

std::optional<std::string>
make_home_assistant_device_discovery_message(const device& device) {
  using R = std::optional<std::string>;
  return std::visit(
      overloaded{
          [](const battery_device& d) -> R {
            return make_battery_discovery(d);
          },
          [](const tank_device& d) -> R { return make_tank_discovery(d); },
          [](const temperature_device& d) -> R {
            return make_temperature_discovery(d);
          },
          [](const voltage_device& d) -> R {
            return make_voltage_discovery(d);
          },
          [](const current_device& d) -> R {
            return make_current_discovery(d);
          },
          [](const auto& d) -> R { return std::nullopt; },
      },
      device);
}

namespace {

std::string make_battery_sensor_message(const battery_device& battery) {
  constexpr auto format_string = R"(
{{
    "battery": {},
    "current": {},
    "voltage": {}
}}
)";

  return std::format(format_string, battery.charge_sensor.value,
                     battery.current_sensor.value,
                     battery.voltage_sensor.value);
}

std::string make_tank_sensor_message(const tank_device& tank) {
  constexpr auto format_string = R"(
{{
    "volume": {},
    "level": {}
}}
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