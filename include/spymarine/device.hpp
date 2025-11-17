#pragma once

#include "spymarine/overloaded.hpp"
#include "spymarine/sensor.hpp"

#include <cstdint>
#include <string>
#include <variant>

namespace spymarine {

struct pico_internal_device {
  static constexpr std::string_view name = "Pico internal";

  sensor device_sensor;

  explicit constexpr pico_internal_device(uint8_t state_start_index,
                                          float sensor_value = 0.0f) noexcept
      : device_sensor{sensor_type::voltage, state_start_index, sensor_value} {}

  constexpr auto
  operator<=>(const pico_internal_device&) const noexcept = default;
};

struct voltage_device {
  std::string name;

  sensor device_sensor;

  constexpr voltage_device(std::string name_, uint8_t state_start_index,
                           float sensor_value = 0.0f) noexcept
      : name{std::move(name_)},
        device_sensor{sensor_type::voltage, state_start_index, sensor_value} {}

  constexpr auto operator<=>(const voltage_device&) const noexcept = default;
};

struct current_device {
  std::string name;

  sensor device_sensor;

  constexpr current_device(std::string name_, uint8_t state_start_index,
                           float sensor_value = 0.0f) noexcept
      : name{std::move(name_)},
        device_sensor{sensor_type::current, state_start_index, sensor_value} {}

  constexpr auto operator<=>(const current_device&) const noexcept = default;
};

struct temperature_device {
  std::string name;

  sensor device_sensor;

  constexpr temperature_device(std::string name_, uint8_t state_start_index,
                               float sensor_value = 0.0f) noexcept
      : name{std::move(name_)}, device_sensor{sensor_type::temperature,
                                              state_start_index, sensor_value} {
  }

  constexpr auto
  operator<=>(const temperature_device&) const noexcept = default;
};

struct barometer_device {
  std::string name;

  sensor device_sensor;

  constexpr barometer_device(std::string name_, uint8_t state_start_index,
                             float sensor_value = 0.0f) noexcept
      : name{std::move(name_)},
        device_sensor{sensor_type::pressure, state_start_index, sensor_value} {}

  constexpr auto operator<=>(const barometer_device&) const noexcept = default;
};

struct resistive_device {
  std::string name;

  sensor device_sensor;

  constexpr resistive_device(std::string name_, uint8_t state_start_index,
                             float sensor_value = 0.0f) noexcept
      : name{std::move(name_)},
        device_sensor{sensor_type::resistive, state_start_index, sensor_value} {
  }

  constexpr auto operator<=>(const resistive_device&) const noexcept = default;
};

enum class fluid_type {
  fresh_water,
  fuel,
  waste_water,
  unknown,
};

struct tank_device {
  std::string name;
  fluid_type type;
  float capacity;

  sensor volume_sensor;
  sensor level_sensor;

  constexpr tank_device(std::string name_, fluid_type type_, float capacity_,
                        uint8_t state_start_index, float volume = 0.0f,
                        float level = 0.0f) noexcept
      : name{std::move(name_)}, type{type_}, capacity{capacity_},
        volume_sensor{sensor_type::volume, state_start_index, volume},
        level_sensor{sensor_type::level, state_start_index, level} {}

  constexpr auto operator<=>(const tank_device&) const noexcept = default;
};

enum class battery_type {
  wet_low_maintenance,
  wet_maintenance_free,
  agm,
  deep_cycle,
  gel,
  lifepo4,
  unknown,
};

struct battery_device {
  std::string name;
  battery_type type;
  float capacity;

  sensor charge_sensor;
  sensor remaining_capacity_sensor;
  sensor current_sensor;
  sensor voltage_sensor;

  constexpr battery_device(std::string name_, battery_type type_,
                           float capacity_, uint8_t state_start_index,
                           float charge = 0.0f, float remaining_capacity = 0.0f,
                           float current = 0.0f, float voltage = 0.0f) noexcept
      : name{std::move(name_)}, type{type_}, capacity{capacity_},
        charge_sensor{sensor_type::charge, state_start_index, charge},
        remaining_capacity_sensor{sensor_type::capacity, state_start_index,
                                  remaining_capacity},
        current_sensor{sensor_type::current, uint8_t(state_start_index + 1),
                       current},
        voltage_sensor{sensor_type::voltage, uint8_t(state_start_index + 2),
                       voltage} {}

  constexpr auto operator<=>(const battery_device&) const noexcept = default;
};

using device =
    std::variant<pico_internal_device, voltage_device, current_device,
                 temperature_device, barometer_device, resistive_device,
                 tank_device, battery_device>;

constexpr std::string_view device_type(const device& d) noexcept {
  return std::visit(
      overloaded{
          [](const pico_internal_device&) constexpr noexcept {
            return "pico_internal";
          },
          [](const battery_device&) constexpr noexcept { return "battery"; },
          [](const tank_device&) constexpr noexcept { return "tank"; },
          [](const temperature_device&) constexpr noexcept {
            return "temperature";
          },
          [](const voltage_device&) constexpr noexcept { return "voltage"; },
          [](const current_device&) constexpr noexcept { return "current"; },
          [](const barometer_device&) constexpr noexcept {
            return "barometer";
          },
          [](const resistive_device&) constexpr noexcept {
            return "resistive";
          },
      },
      d);
}

constexpr uint8_t device_id(const device& d_) noexcept {
  return std::visit(overloaded{
                        [](const battery_device& d) constexpr noexcept {
                          return d.charge_sensor.state_index;
                        },
                        [](const tank_device& d) constexpr noexcept {
                          return d.volume_sensor.state_index;
                        },
                        [](const auto& d) constexpr noexcept {
                          return d.device_sensor.state_index;
                        },
                    },
                    d_);
}

} // namespace spymarine
