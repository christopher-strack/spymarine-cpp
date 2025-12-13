#pragma once

#include "spymarine/device.hpp"
#include "spymarine/hub.hpp"
#include "spymarine/overloaded.hpp"
#include "spymarine/sensor.hpp"
#include "spymarine/system_info.hpp"
#include "spymarine/unit.hpp"

#include <concepts> // IWYU pragma: keep
#include <format>
#include <ranges>
#include <string>
#include <unordered_map>

namespace spymarine {

struct mqtt_message {
  std::string topic;
  std::string payload;

  auto operator<=>(const mqtt_message&) const = default;
};

struct home_assistant_state_config {
  // Report the average value for current and voltage sensors instead of the
  // instantaneous value. This is useful for Home Assistant integrations that
  // do not support frequent updates, as it reflects changes between updates
  // better.
  bool use_average_value_for_current_and_voltage = false;
};

struct home_assistant_origin {
  std::string_view name;
  std::string_view sw_version;
  std::string_view support_url;
};

struct home_assistant_device {
  std::string identifiers;
  std::optional<std::string> name;
  std::string_view manufacturer;
  uint32_t serial_number;
  firmware_version fw_version;
};

struct home_assistant_device_component {
  std::string component_id;
  std::string_view platform;
  std::optional<std::string_view> device_class;
  std::optional<std::string_view> name;
  std::string_view unit_of_measurement;
  std::string value_template;
  std::string unique_id;
};

struct home_assistant_device_discovery {
  home_assistant_device device;
  home_assistant_origin origin;
  std::vector<home_assistant_device_component> components;
  std::string state_topic;
};

constexpr std::string quote_string(const std::string& str) {
  return std::format("\"{}\"", str);
}

constexpr std::string to_json_object(const std::ranges::range auto& entries) {
  std::string result = "{";
  bool first = true;
  for (const auto& entry : entries) {
    if (!first) {
      result += ",";
    } else {
      first = false;
    }

    result += std::format("\"{}\":{}", std::get<0>(entry), std::get<1>(entry));
  }
  result += "}";

  return result;
}

constexpr std::string to_json(const home_assistant_origin& origin) {
  return std::format(R"({{"name":"{}","sw":"{}","url":"{}"}})", origin.name,
                     origin.sw_version, origin.support_url);
}

constexpr std::string to_json(const home_assistant_device& dev) {
  const auto name_or_null = dev.name.transform(quote_string).value_or("null");
  return std::format(
      R"({{"ids":"{}","name":{},"mf":"{}","sn":"{}","sw":"{}.{}"}})",
      dev.identifiers, name_or_null, dev.manufacturer, dev.serial_number,
      dev.fw_version.major, dev.fw_version.minor);
}

constexpr std::string
to_json(const home_assistant_device_component& component) {
  const auto device_class_str =
      component.device_class
          ? std::format(R"("dev_cla":"{}",)", *component.device_class)
          : "";

  const auto name_str =
      component.name ? std::format(R"("name":"{}",)", *component.name) : "";

  return std::format(
      R"({{"p":"{}",{}{}"unit_of_meas":"{}","val_tpl":"{}","uniq_id":"{}"}})",
      component.platform, device_class_str, name_str,
      component.unit_of_measurement, component.value_template,
      component.unique_id);
}

constexpr std::string
to_json(const std::vector<home_assistant_device_component>& components) {
  return to_json_object(components | std::views::transform([](const auto& c) {
                          return std::make_tuple(c.component_id, to_json(c));
                        }));
}

constexpr std::string
to_json(const home_assistant_device_discovery& discovery) {
  return std::format(R"({{"dev":{},"o":{},"cmps":{},"stat_t":"{}","qos":1}})",
                     to_json(discovery.device), to_json(discovery.origin),
                     to_json(discovery.components), discovery.state_topic);
}

constexpr std::string_view to_home_assistant_unit(const unit u) {
  switch (u) {
  case unit::volts:
    return "V";
  case unit::amps:
    return "A";
  case unit::celsius:
    return "°C";
  case unit::millibars_per_hour:
    return "mbar/h";
  case unit::millibar:
    return "mbar";
  case unit::ohms:
    return "Ω";
  case unit::liters:
    return "L";
  case unit::percent:
    return "%";
  case unit::amp_hours:
    return "Ah";
  }
  return "";
}

constexpr std::string_view
get_short_device_type_name(const device& device_) noexcept {
  return std::visit(overloaded{
                        [](const voltage_device&) { return "volt"; },
                        [](const current_device&) { return "cur"; },
                        [](const temperature_device&) { return "temp"; },
                        [](const barometer_device&) { return "baro"; },
                        [](const resistive_device&) { return "res"; },
                        [](const tank_device&) { return "tank"; },
                        [](const battery_device&) { return "batt"; },
                        [](const unsupported_device&) { return "unsupp"; },
                    },
                    device_);
}

constexpr home_assistant_origin make_home_assistant_origin() {
  return home_assistant_origin{
      .name = "spymarine",
      .sw_version = SPYMARINE_PROJECT_VERSION,
      .support_url = "https://github.com/christopher-strack/spymarine-cpp",
  };
}

constexpr auto
home_assistant_device_identifier(const auto& device_,
                                 const system_info& system_info_) {
  return std::format("simarine.{}.{}{}", system_info_.serial_number,
                     get_short_device_type_name(device_), device_.id);
}

constexpr home_assistant_device
make_home_assistant_device(const auto& device_, const system_info& system_info_,
                           std::string identifier) {
  return home_assistant_device{
      .identifiers = std::move(identifier),
      .name = device_.name,
      .manufacturer = "Simarine",
      .serial_number = system_info_.serial_number,
      .fw_version = system_info_.fw_version,
  };
}

constexpr std::vector<home_assistant_device_component>
make_home_assistant_sensor_components(const sensor& sensor_,
                                      const std::string& device_identifier) {
  std::vector<home_assistant_device_component> components;

  std::visit(
      overloaded{
          [&](const voltage_sensor& s) {
            const auto id = std::format("{}{}", "volt", s.id);
            components.emplace_back(home_assistant_device_component{
                .component_id = id,
                .platform = "sensor",
                .device_class = "voltage",
                .name = std::nullopt,
                .unit_of_measurement = to_home_assistant_unit(s.value.unit),
                .value_template = "{{ value_json.volt }}",
                .unique_id = std::format("{}.{}", device_identifier, id),
            });
          },
          [&](const current_sensor& s) {
            const auto id = std::format("{}{}", "cur", s.id);
            components.emplace_back(home_assistant_device_component{
                .component_id = id,
                .platform = "sensor",
                .device_class = "current",
                .name = std::nullopt,
                .unit_of_measurement = to_home_assistant_unit(s.value.unit),
                .value_template = "{{ value_json.cur }}",
                .unique_id = std::format("{}.{}", device_identifier, id),
            });
          },
          [&](const temperature_sensor& s) {
            const auto id = std::format("{}{}", "temp", s.id);
            components.emplace_back(home_assistant_device_component{
                .component_id = id,
                .platform = "sensor",
                .device_class = "temperature",
                .name = std::nullopt,
                .unit_of_measurement = to_home_assistant_unit(s.value.unit),
                .value_template = "{{ value_json.temp }}",
                .unique_id = std::format("{}.{}", device_identifier, id),
            });
          },
          [&](const barometer_trend_sensor&) {},
          [&](const barometer_sensor& s) {
            const auto id = std::format("{}{}", "baro", s.id);
            components.emplace_back(home_assistant_device_component{
                .component_id = id,
                .platform = "sensor",
                .device_class = "atmospheric_pressure",
                .name = std::nullopt,
                .unit_of_measurement = to_home_assistant_unit(s.value.unit),
                .value_template = "{{ value_json.baro }}",
                .unique_id = std::format("{}.{}", device_identifier, id),
            });
          },
          [&](const resistive_sensor& s) {
            const auto id = std::format("{}{}", "res", s.id);
            components.emplace_back(home_assistant_device_component{
                .component_id = id,
                .platform = "sensor",
                .device_class = std::nullopt,
                .name = "Resistive Sensor",
                .unit_of_measurement = to_home_assistant_unit(s.value.unit),
                .value_template = "{{ value_json.res }}",
                .unique_id = std::format("{}.{}", device_identifier, id),
            });
          },
          [&](const tank_sensor& s) {
            const auto vol_id = std::format("{}{}", "vol", s.id);
            const auto lvl_id = std::format("{}{}", "lvl", s.id);
            components.emplace_back(home_assistant_device_component{
                .component_id = vol_id,
                .platform = "sensor",
                .device_class = "volume_storage",
                .name = std::nullopt,
                .unit_of_measurement = to_home_assistant_unit(s.volume.unit),
                .value_template = "{{ value_json.vol }}",
                .unique_id = std::format("{}.{}", device_identifier, vol_id),
            });
            components.emplace_back(home_assistant_device_component{
                .component_id = lvl_id,
                .platform = "sensor",
                .device_class = std::nullopt,
                .name = "Tank Level",
                .unit_of_measurement = to_home_assistant_unit(s.level.unit),
                .value_template = "{{ value_json.lvl }}",
                .unique_id = std::format("{}.{}", device_identifier, lvl_id),
            });
          },
          [&](const battery_sensor& s) {
            const auto batt_it = std::format("{}{}", "batt", s.id);
            const auto cap_it = std::format("{}{}", "cap", s.id);
            components.emplace_back(home_assistant_device_component{
                .component_id = batt_it,
                .platform = "sensor",
                .device_class = "battery",
                .name = std::nullopt,
                .unit_of_measurement = to_home_assistant_unit(s.charge.unit),
                .value_template = "{{ value_json.batt }}",
                .unique_id = std::format("{}.{}", device_identifier, batt_it),
            });
            components.emplace_back(home_assistant_device_component{
                .component_id = cap_it,
                .platform = "sensor",
                .device_class = std::nullopt,
                .name = "Remaining Capacity",
                .unit_of_measurement =
                    to_home_assistant_unit(s.remaining_capacity.unit),
                .value_template = "{{ value_json.cap }}",
                .unique_id = std::format("{}.{}", device_identifier, cap_it),
            });
          },
          [&](const unsupported_sensor&) {},
      },
      sensor_);

  return components;
}

constexpr std::vector<home_assistant_device_component>
make_home_assistant_device_sensor_components(
    const device& device_, const sensor_range auto& sensors,
    const std::string& device_identifier) {
  return get_sensors(device_, sensors) |
         std::views::transform([&](const auto& sen) {
           return make_home_assistant_sensor_components(sen, device_identifier);
         }) |
         std::views::join | std::ranges::to<std::vector>();
}

constexpr std::string
make_home_assistant_device_discovery_topic(const device& device_,
                                           const uint32_t serial_number) {
  return std::format("homeassistant/device/simarine_{}_{}{}/config",
                     serial_number, get_short_device_type_name(device_),
                     get_device_id(device_));
}

constexpr std::string make_home_assistant_state_topic(const device& device_,
                                                      uint32_t serial_number) {
  return std::format("simarine/{}/{}{}/state", serial_number,
                     get_short_device_type_name(device_),
                     get_device_id(device_));
}

constexpr home_assistant_device_discovery
make_home_assistant_device_discovery(const device& device_,
                                     const sensor_range auto& sensors,
                                     const system_info& system_info_) {
  return std::visit(
      [&](const auto& dev) {
        const auto ha_device_id =
            home_assistant_device_identifier(dev, system_info_);
        return home_assistant_device_discovery{
            .device =
                make_home_assistant_device(dev, system_info_, ha_device_id),
            .origin = make_home_assistant_origin(),
            .components = make_home_assistant_device_sensor_components(
                device_, sensors, ha_device_id),
            .state_topic = make_home_assistant_state_topic(
                dev, system_info_.serial_number),
        };
      },
      device_);
}

inline std::unordered_map<std::string, std::string>
make_home_assistant_device_sensor_states(
    const device& device_, const sensor_range auto& sensors,
    const home_assistant_state_config& config) {
  std::unordered_map<std::string, std::string> entries;

  for (const auto& sensor_ : get_sensors(device_, sensors)) {
    std::visit(
        overloaded{
            [&](const voltage_sensor& s) {
              entries.emplace("volt",
                              config.use_average_value_for_current_and_voltage
                                  ? std::to_string(s.value.average_value)
                                  : s.value.current_value.to_string());
            },
            [&](const current_sensor& s) {
              entries.emplace("cur",
                              config.use_average_value_for_current_and_voltage
                                  ? std::to_string(s.value.average_value)
                                  : s.value.current_value.to_string());
            },
            [&](const temperature_sensor& s) {
              entries.emplace("temp", s.value.current_value.to_string());
            },
            [&](const barometer_trend_sensor&) {},
            [&](const barometer_sensor& s) {
              entries.emplace("baro", s.value.current_value.to_string());
            },
            [&](const resistive_sensor& s) {
              entries.emplace("res", s.value.current_value.to_string());
            },
            [&](const tank_sensor& s) {
              entries.emplace("vol", s.volume.current_value.to_string());
              entries.emplace("lvl", s.level.current_value.to_string());
            },
            [&](const battery_sensor& s) {
              entries.emplace("batt", s.charge.current_value.to_string());
              entries.emplace("cap",
                              s.remaining_capacity.current_value.to_string());
            },
            [&](const unsupported_sensor&) {},
        },
        sensor_);
  };

  return entries;
}

template <typename tcp_socket_type, typename udp_socket_type>
constexpr mqtt_message make_home_assistant_device_discovery_message(
    const device& device_,
    const basic_hub<tcp_socket_type, udp_socket_type>& hub_) {
  return mqtt_message{
      .topic = make_home_assistant_device_discovery_topic(
          device_, hub_.system().serial_number),
      .payload = to_json(make_home_assistant_device_discovery(
          device_, hub_.all_sensors(), hub_.system())),
  };
}

template <typename tcp_socket_type, typename udp_socket_type>
inline mqtt_message make_home_assistant_state_message(
    const device& device_,
    const basic_hub<tcp_socket_type, udp_socket_type>& hub_,
    const home_assistant_state_config& config) {

  return mqtt_message{
      .topic =
          make_home_assistant_state_topic(device_, hub_.system().serial_number),
      .payload = to_json_object(make_home_assistant_device_sensor_states(
          device_, hub_.all_sensors(), config)),
  };
}

} // namespace spymarine
