#pragma once

#include "spymarine/device.hpp"
#include "spymarine/device2.hpp"
#include "spymarine/sensor2.hpp"

#include <vector>

namespace spymarine {

inline auto make_parsed_devices() {
  return std::vector<device>{
      barometer_device{"Barometer", 3},
      pico_internal_device{5},
      voltage_device{"ST107 [5596] 1", 11},
      voltage_device{"ST107 [5596] 2", 12},
      voltage_device{"ST107 [5596] 3", 13},
      resistive_device{"ST107 [5596] 1", 14},
      resistive_device{"ST107 [5596] 2", 15},
      resistive_device{"ST107 [5596] 3", 16},
      resistive_device{"ST107 [5596] 4", 17},
      current_device{"SC303 [5499]", 19},
      voltage_device{"SC303 [5499] 1", 21},
      voltage_device{"SC303 [5499] 2", 22},
      resistive_device{"SC303 [5499] 1", 23},
      resistive_device{"SC303 [5499] 2", 24},
      resistive_device{"SC303 [5499] 3", 25},
      battery_device{"Bulltron", battery_type::lifepo4, 300.0f, 26},
      temperature_device{"Batterie", 31},
      tank_device{"Frischwasser", fluid_type::fresh_water, 100.0f, 32},
      battery_device{"Starterbatterie", battery_type::agm, 100.0f, 33},
      tank_device{"Abwasser", fluid_type::waste_water, 70.0f, 38},
      temperature_device{"Innen", 39},
      temperature_device{"Au?en ", 40},
      temperature_device{"Boiler", 41},
  };
}

inline auto make_parsed_devices_with_values() {
  return std::vector<device>{
      barometer_device{"Barometer", 3, 979.83f},
      pico_internal_device{5, 13.26f},
      voltage_device{"ST107 [5596] 1", 11, 0},
      voltage_device{"ST107 [5596] 2", 12, 0.035f},
      voltage_device{"ST107 [5596] 3", 13, 0},
      resistive_device{"ST107 [5596] 1", 14, 19121},
      resistive_device{"ST107 [5596] 2", 15, 20599},
      resistive_device{"ST107 [5596] 3", 16, 5479},
      resistive_device{"ST107 [5596] 4", 17, -1},
      current_device{"SC303 [5499]", 19, -1.23f},
      voltage_device{"SC303 [5499] 1", 21, 13.314f},
      voltage_device{"SC303 [5499] 2", 22, 12.26f},
      resistive_device{"SC303 [5499] 1", 23, -1},
      resistive_device{"SC303 [5499] 2", 24, -1},
      resistive_device{"SC303 [5499] 3", 25, 19651},
      battery_device{"Bulltron", battery_type::lifepo4, 300.0f, 26, 87.9f,
                     263.7f, -1.23f, 13.314f},
      temperature_device{"Batterie", 31, 10.1f},
      tank_device{"Frischwasser", fluid_type::fresh_water, 100.0f, 32, 0.0f,
                  0.0f},
      battery_device{"Starterbatterie", battery_type::agm, 100.0f, 33, 46.9f,
                     79.6f, -0.01f, 12.26f},
      tank_device{"Abwasser", fluid_type::waste_water, 70.0f, 38, 3.7f, 5.2f},
      temperature_device{"Innen", 39, 10.7f},
      temperature_device{"Au?en ", 40, 9.1f},
      temperature_device{"Boiler", 41, 39.4f},
  };
}

constexpr auto make_parsed_devices2() {
  return std::to_array<device2>({
      unsupported_device2{0, 10, std::nullopt, {}},
      unsupported_device2{1, 7, std::nullopt, {}},
      unsupported_device2{2, 4, "Sensor 1", {}},
      unsupported_device2{3, 0, std::nullopt, {}},
      unsupported_device2{4, 0, std::nullopt, {}},
      barometer_device2{5, "Barometer", {}},
      voltage_device2{6, "PICO INTERNAL", {}},
      unsupported_device2{7, 0, std::nullopt, {}},
      unsupported_device2{8, 0, std::nullopt, {}},
      unsupported_device2{9, 0, std::nullopt, {}},
      voltage_device2{10, "ST107 [5596] 1", {}},
      voltage_device2{11, "ST107 [5596] 2", {}},
      voltage_device2{12, "ST107 [5596] 3", {}},
      resistive_device2{13, "ST107 [5596] 1", {}},
      resistive_device2{14, "ST107 [5596] 2", {}},
      resistive_device2{15, "ST107 [5596] 3", {}},
      resistive_device2{16, "ST107 [5596] 4", {}},
      unsupported_device2{17, 14, "ST107 [5596]", {}},
      current_device2{18, "SC303 [5499]", {}},
      voltage_device2{19, "SC303 [5499] 1", {}},
      voltage_device2{20, "SC303 [5499] 2", {}},
      resistive_device2{21, "SC303 [5499] 1", {}},
      resistive_device2{22, "SC303 [5499] 2", {}},
      resistive_device2{23, "SC303 [5499] 3", {}},
      battery_device2{
          24, "Bulltron", battery_type::lifepo4, battery_capacity{300'00}, {}},
      temperature_device2{25, "Batterie", {}},
      tank_device2{26,
                   "Frischwasser",
                   fluid_type::fresh_water,
                   tank_capacity{100'0},
                   {}},
      battery_device2{27,
                      "Starterbatterie",
                      battery_type::agm,
                      battery_capacity{100'00},
                      {}},
      tank_device2{
          28, "Abwasser", fluid_type::waste_water, tank_capacity{70'0}, {}},
      temperature_device2{29, "Innen", {}},
      temperature_device2{30, "Au?en ", {}},
      temperature_device2{31, "Boiler", {}},
  });
}

constexpr auto parsed_sensors2 = std::to_array<sensor2>({
    unsupported_sensor2{.id = 0, .parent_device_id = 1, .raw_type = 10},
    unsupported_sensor2{.id = 1, .parent_device_id = 1, .raw_type = 10},
    barometer_sensor2{.id = 2, .parent_device_id = 5},
    barometer_sensor2{.id = 3, .parent_device_id = 5},
    unsupported_sensor2{.id = 4, .parent_device_id = 5, .raw_type = 6},
    voltage_sensor2{.id = 5, .parent_device_id = 6},
    unsupported_sensor2{
        .id = 6, .parent_device_id = std::nullopt, .raw_type = 0},
    unsupported_sensor2{
        .id = 7, .parent_device_id = std::nullopt, .raw_type = 0},
    unsupported_sensor2{
        .id = 8, .parent_device_id = std::nullopt, .raw_type = 0},
    unsupported_sensor2{.id = 9, .parent_device_id = 1, .raw_type = 10},
    barometer_sensor2{.id = 10, .parent_device_id = 2},
    voltage_sensor2{.id = 11, .parent_device_id = 10},
    voltage_sensor2{.id = 12, .parent_device_id = 11},
    voltage_sensor2{.id = 13, .parent_device_id = 12},
    resistive_sensor2{.id = 14, .parent_device_id = 13},
    resistive_sensor2{.id = 15, .parent_device_id = 14},
    resistive_sensor2{.id = 16, .parent_device_id = 15},
    resistive_sensor2{.id = 17, .parent_device_id = 16},
    unsupported_sensor2{.id = 18, .parent_device_id = 17, .raw_type = 15},
    current_sensor2{.id = 19, .parent_device_id = 18},
    unsupported_sensor2{.id = 20, .parent_device_id = 18, .raw_type = 3},
    voltage_sensor2{.id = 21, .parent_device_id = 19},
    voltage_sensor2{.id = 22, .parent_device_id = 20},
    resistive_sensor2{.id = 23, .parent_device_id = 21},
    resistive_sensor2{.id = 24, .parent_device_id = 22},
    resistive_sensor2{.id = 25, .parent_device_id = 23},
    battery_sensor2{.id = 26, .parent_device_id = 24},
    current_sensor2{.id = 27, .parent_device_id = 24},
    voltage_sensor2{.id = 28, .parent_device_id = 24},
    unsupported_sensor2{.id = 29, .parent_device_id = 24, .raw_type = 12},
    unsupported_sensor2{.id = 30, .parent_device_id = 24, .raw_type = 13},
    temperature_sensor2{.id = 31, .parent_device_id = 25},
    tank_sensor2{.id = 32, .parent_device_id = 26},
    battery_sensor2{.id = 33, .parent_device_id = 27},
    current_sensor2{.id = 34, .parent_device_id = 27},
    voltage_sensor2{.id = 35, .parent_device_id = 27},
    unsupported_sensor2{.id = 36, .parent_device_id = 27, .raw_type = 12},
    unsupported_sensor2{.id = 37, .parent_device_id = 27, .raw_type = 13},
    tank_sensor2{.id = 38, .parent_device_id = 28},
    temperature_sensor2{.id = 39, .parent_device_id = 29},
    temperature_sensor2{.id = 40, .parent_device_id = 30},
    temperature_sensor2{.id = 41, .parent_device_id = 31},
});

} // namespace spymarine
