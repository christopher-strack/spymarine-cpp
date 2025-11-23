#pragma once

#include "spymarine/device_info.hpp"
#include "spymarine/sensor2.hpp"

namespace spymarine {

struct pico_internal_device2 {
  pico_internal_device_info info;

  sensor2<voltage_type> voltage_sensor;

  constexpr pico_internal_device2(pico_internal_device_info info,
                                  sensor_id id) noexcept
      : info{info}, voltage_sensor{id} {}

  constexpr auto
  operator<=>(const pico_internal_device2&) const noexcept = default;
};

struct voltage_device2 {
  voltage_device_info info;

  sensor2<voltage_type> sensor;

  constexpr voltage_device2(voltage_device_info info, sensor_id id) noexcept
      : info{info}, sensor{id} {}

  constexpr auto operator<=>(const voltage_device2&) const noexcept = default;
};

struct current_device2 {
  current_device_info info;

  sensor2<current_type> sensor;

  constexpr current_device2(current_device_info info, sensor_id id) noexcept
      : info{info}, sensor{id} {}

  constexpr auto operator<=>(const current_device2&) const noexcept = default;
};

struct temperature_device2 {
  temperature_device_info info;

  sensor2<temperature_type> sensor;

  constexpr temperature_device2(temperature_device_info info,
                                sensor_id id) noexcept
      : info{info}, sensor{id} {}

  constexpr auto
  operator<=>(const temperature_device2&) const noexcept = default;
};

struct barometer_device2 {
  barometer_device_info info;

  sensor2<barometer_type> sensor;

  constexpr barometer_device2(barometer_device_info info, sensor_id id) noexcept
      : info{info}, sensor{id} {}

  constexpr auto operator<=>(const barometer_device2&) const noexcept = default;
};

struct resistive_device2 {
  resistive_device_info info;

  sensor2<resistive_type> sensor;

  constexpr resistive_device2(resistive_device_info info, sensor_id id) noexcept
      : info{info}, sensor{id} {}

  constexpr auto operator<=>(const resistive_device2&) const noexcept = default;
};

struct tank_device2 {
  tank_device_info info;

  sensor2<tank_type> sensor;

  constexpr tank_device2(tank_device_info info, sensor_id id) noexcept
      : info{info}, sensor{id} {}

  constexpr auto operator<=>(const tank_device2&) const noexcept = default;
};

struct battery_device2 {
  battery_device_info info;

  sensor2<battery_charge_type> charge_sensor;
  sensor2<current_type> current_sensor;
  sensor2<voltage_type> voltage_sensor;

  constexpr battery_device2(battery_device_info info, sensor_id id) noexcept
      : info{info}, charge_sensor{id}, current_sensor{id + 1},
        voltage_sensor{id + 2} {}

  constexpr auto operator<=>(const battery_device2&) const noexcept = default;
};

} // namespace spymarine
