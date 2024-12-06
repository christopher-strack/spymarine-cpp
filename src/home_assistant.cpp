#include "spymarine/home_assistant.hpp"
#include "spymarine/device.hpp"
#include "spymarine/overloaded.hpp"

#include <format>
#include <string>

namespace spymarine {
namespace {
constexpr std::string make_device_object(const uint8_t device_id,
                                         const std::string_view type,
                                         const std::string_view name) {
  constexpr auto format_string =
      R"({{"ids":"simarine_{}_{}","name":"{}","mf":"Simarine"}})";
  return std::format(format_string, type, device_id, name);
}

constexpr std::string make_sensor_component_entry(
    const uint8_t device_id, const std::string_view type,
    const std::string_view device_class, const std::string_view unit,
    const std::string_view template_name) {
  constexpr auto format_string =
      R"("{}":{{"p":"sensor","device_class":"{}","unit_of_measurement":"{}","value_template":"{{{{ value_json.{} }}}}","unique_id":"{}"}})";
  const auto unique_id = std::format("{}_{}", type, device_id);
  return std::format(format_string, unique_id, device_class, unit,
                     template_name, unique_id);
}

constexpr std::string
make_generic_sensor_component_entry(const uint8_t device_id,
                                    const std::string_view type,
                                    const std::string_view unit) {
  constexpr auto format_string =
      R"("{}":{{"p":"sensor","unit_of_measurement":"{}","value_template":"{{{{ value_json.{} }}}}","unique_id":"{}"}})";
  const auto unique_id = std::format("{}_{}", type, device_id);
  return std::format(format_string, unique_id, unit, type, unique_id);
}

constexpr std::string_view origin_object() {
  return R"({"name":"spymarine","sw":")" PROJECT_VERSION
         R"(","url":"https://github.com/christopher-strack/spymarine-cpp"})";
}

std::string make_home_assistant_state_topic(const device& device) {
  return std::format("simarine_{}_{}/state", device_type(device),
                     device_id(device));
}

std::string make_battery_discovery(const battery_device& battery) {
  constexpr auto format_string =
      R"({{"dev":{},"o":{},"cmps":{{{},{},{}}},"state_topic":"{}","qos":1}})";

  const auto device_id = battery.charge_sensor.state_index;

  return std::format(
      format_string, make_device_object(device_id, "battery", battery.name),
      origin_object(),
      make_sensor_component_entry(battery.charge_sensor.state_index, "battery",
                                  "battery", "%", "battery"),
      make_sensor_component_entry(battery.current_sensor.state_index, "current",
                                  "current", "A", "current"),
      make_sensor_component_entry(battery.voltage_sensor.state_index, "voltage",
                                  "voltage", "V", "voltage"),
      make_home_assistant_state_topic(battery));
}

std::string make_tank_discovery(const tank_device& tank) {
  constexpr auto format_string =
      R"({{"dev":{},"o":{},"cmps":{{{},{}}},"state_topic":"{}","qos":1}})";

  const auto device_id = tank.volume_sensor.state_index;

  return std::format(
      format_string, make_device_object(device_id, "tank", tank.name),
      origin_object(),
      make_sensor_component_entry(tank.volume_sensor.state_index, "volume",
                                  "volume", "L", "volume"),
      make_generic_sensor_component_entry(tank.level_sensor.state_index,
                                          "level", "%"),
      make_home_assistant_state_topic(tank));
}

std::string make_sensor_discovery(const auto& device, std::string_view type,
                                  std::string_view device_class,
                                  std::string_view unit) {
  constexpr auto format_string =
      R"({{"dev":{},"o":{},"cmps":{{{}}},"state_topic":"{}","qos":1}})";

  const auto device_id = device.device_sensor.state_index;

  return std::format(
      format_string, make_device_object(device_id, type, device.name),
      origin_object(),
      make_sensor_component_entry(device.device_sensor.state_index, type,
                                  device_class, unit, "value"),
      make_home_assistant_state_topic(device));
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

mqtt_message
make_home_assistant_device_discovery_message(const device& device) {
  return mqtt_message{
      .topic = make_home_assistant_device_discovery_topic(device),
      .payload = make_home_assistant_device_discovery_payload(device),
  };
}

mqtt_message make_home_assistant_state_message(const device& device) {
  return mqtt_message{
      .topic = make_home_assistant_state_topic(device),
      .payload = std::visit(
          overloaded{
              [](const battery_device& battery) {
                constexpr auto format_string =
                    R"({{"battery":{},"current":{},"voltage":{}}})";

                return std::format(format_string, battery.charge_sensor.value,
                                   battery.current_sensor.value,
                                   battery.voltage_sensor.value);
              },
              [](const tank_device& tank) {
                constexpr auto format_string = R"({{"volume":{},"level":{}}})";

                return std::format(format_string, tank.volume_sensor.value,
                                   tank.level_sensor.value);
              },
              [](const auto& device) {
                constexpr auto format_string = R"({{"value":{}}})";
                return std::format(format_string, device.device_sensor.value);
              },
          },
          device),
  };
}

} // namespace spymarine
