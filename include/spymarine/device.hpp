#pragma once

#include "spymarine/sensor.hpp"

#include <expected>
#include <string>
#include <variant>

namespace spymarine {

struct pico_internal_device {
  static constexpr std::string_view name = "Pico internal";

  sensor device_sensor;

  explicit pico_internal_device(uint8_t state_start_index,
                                float sensor_value = 0.0f);

  auto operator<=>(const pico_internal_device&) const = default;
};

struct voltage_device {
  std::string name;

  sensor device_sensor;

  voltage_device(std::string name, uint8_t state_start_index,
                 float sensor_value = 0.0f);

  auto operator<=>(const voltage_device&) const = default;
};

struct current_device {
  std::string name;

  sensor device_sensor;

  current_device(std::string name, uint8_t state_start_index,
                 float sensor_value = 0.0f);

  auto operator<=>(const current_device&) const = default;
};

struct temperature_device {
  std::string name;

  sensor device_sensor;

  temperature_device(std::string name, uint8_t state_start_index,
                     float sensor_value = 0.0f);

  auto operator<=>(const temperature_device&) const = default;
};

struct barometer_device {
  std::string name;

  sensor device_sensor;

  barometer_device(std::string name, uint8_t state_start_index,
                   float sensor_value = 0.0f);

  auto operator<=>(const barometer_device&) const = default;
};

struct resistive_device {
  std::string name;

  sensor device_sensor;

  resistive_device(std::string name, uint8_t state_start_index,
                   float sensor_value = 0.0f);

  auto operator<=>(const resistive_device&) const = default;
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

  tank_device(std::string name, fluid_type type, float capacity,
              uint8_t state_start_index, float volume = 0.0f,
              float level = 0.0f);

  auto operator<=>(const tank_device&) const = default;
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

  battery_device(std::string name, battery_type type, float capacity,
                 uint8_t state_start_index, float charge = 0.0f,
                 float remaining_capacity = 0.0f, float current = 0.0f,
                 float voltage = 0.0f);

  auto operator<=>(const battery_device&) const = default;
};

using device =
    std::variant<pico_internal_device, voltage_device, current_device,
                 temperature_device, barometer_device, resistive_device,
                 tank_device, battery_device>;

std::string_view device_type(const device& device);

uint8_t device_id(const device& device);

} // namespace spymarine
