#pragma once

#include "spymarine/device.hpp"

#include <vector>

namespace spymarine {

const auto parsed_devices = std::vector<device>{
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

const auto parsed_devices_with_values = std::vector<device>{
    barometer_device{"Barometer", 3, 979.83},
    pico_internal_device{5, 13.26},
    voltage_device{"ST107 [5596] 1", 11, 0},
    voltage_device{"ST107 [5596] 2", 12, 0.035f},
    voltage_device{"ST107 [5596] 3", 13, 0},
    resistive_device{"ST107 [5596] 1", 14, 19121},
    resistive_device{"ST107 [5596] 2", 15, 20599},
    resistive_device{"ST107 [5596] 3", 16, 5479},
    resistive_device{"ST107 [5596] 4", 17, -1},
    current_device{"SC303 [5499]", 19, -1.23f},
    voltage_device{"SC303 [5499] 1", 21, 13.314},
    voltage_device{"SC303 [5499] 2", 22, 12.26f},
    resistive_device{"SC303 [5499] 1", 23, -1},
    resistive_device{"SC303 [5499] 2", 24, -1},
    resistive_device{"SC303 [5499] 3", 25, 19651},
    battery_device{"Bulltron", battery_type::lifepo4, 300.0f, 26, 87.9f, 263.7f,
                   -1.23f, 13.314f},
    temperature_device{"Batterie", 31, 10.1},
    tank_device{"Frischwasser", fluid_type::fresh_water, 100.0f, 32, 0.0f,
                0.0f},
    battery_device{"Starterbatterie", battery_type::agm, 100.0f, 33, 46.9f,
                   79.6f, -0.01f, 12.26f},
    tank_device{"Abwasser", fluid_type::waste_water, 70.0f, 38, 3.7f, 5.2f},
    temperature_device{"Innen", 39, 10.7f},
    temperature_device{"Au?en ", 40, 9.1f},
    temperature_device{"Boiler", 41, 39.4f},
};

} // namespace spymarine
