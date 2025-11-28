#pragma once

#include "spymarine/device_info.hpp"
#include "spymarine/overloaded.hpp"
#include "spymarine/sensor2.hpp"

#include <variant>

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

using device2 =
    std::variant<pico_internal_device2, voltage_device2, current_device2,
                 temperature_device2, barometer_device2, resistive_device2,
                 tank_device2, battery_device2>;

constexpr std::optional<device2> make_device(const device_info info,
                                             const sensor_id offset) noexcept {
  return std::visit(
      overloaded{
          [offset](pico_internal_device_info di) -> std::optional<device2> {
            return pico_internal_device2{std::move(di), offset};
          },
          [offset](voltage_device_info di) -> std::optional<device2> {
            return voltage_device2{std::move(di), offset};
          },
          [offset](current_device_info di) -> std::optional<device2> {
            return current_device2{std::move(di), offset};
          },
          [offset](temperature_device_info di) -> std::optional<device2> {
            return temperature_device2{std::move(di), offset};
          },
          [offset](barometer_device_info di) -> std::optional<device2> {
            return barometer_device2{std::move(di), offset};
          },
          [offset](resistive_device_info di) -> std::optional<device2> {
            return resistive_device2{std::move(di), offset};
          },
          [offset](tank_device_info di) -> std::optional<device2> {
            return tank_device2{std::move(di), offset};
          },
          [offset](battery_device_info di) -> std::optional<device2> {
            return battery_device2{std::move(di), offset};
          },
          [](null_device_info) -> std::optional<device2> {
            return std::nullopt;
          },
          [](unknown_device_info) -> std::optional<device2> {
            return std::nullopt;
          },
      },
      std::move(info));
}

} // namespace spymarine
