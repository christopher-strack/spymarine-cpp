#include "spymarine/device_ostream.hpp"

#include <ostream>

namespace spymarine {

std::ostream& operator<<(std::ostream& os, const sensor_type type) {
  switch (type) {
  case sensor_type::volume:
    return os << "volume";
  case sensor_type::level:
    return os << "level";
  case sensor_type::voltage:
    return os << "voltage";
  case sensor_type::current:
    return os << "current";
  case sensor_type::temperature:
    return os << "temperature";
  case sensor_type::pressure:
    return os << "pressure";
  case sensor_type::resistive:
    return os << "resistive";
  case sensor_type::charge:
    return os << "charge";
  case sensor_type::capacity:
    return os << "capacity";
  default:
    return os << "invalid";
  }
}

std::ostream& operator<<(std::ostream& os, const sensor& info) {
  return os << "sensor_info{type=" << info.type
            << ",state_index=" << int(info.state_index)
            << ",value=" << info.value << "}";
}

std::ostream& operator<<(std::ostream& os, const pico_internal_device& device) {
  return os << "pico_internal_device{voltage_sensor=" << device.device_sensor
            << "}";
}

std::ostream& operator<<(std::ostream& os, const voltage_device& device) {
  return os << "voltage_device{name=" << device.name
            << ",voltage_sensor=" << device.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const current_device& device) {
  return os << "current_device{name=" << device.name
            << ",current_sensor=" << device.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const temperature_device& device) {
  return os << "temperature_device{name=" << device.name
            << ",temperature_sensor=" << device.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const barometer_device& device) {
  return os << "barometer_device{name=" << device.name
            << ",pressure_sensor=" << device.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const resistive_device& device) {
  return os << "resistive_device{name=" << device.name
            << ",resistive_sensor=" << device.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const fluid_type type) {
  switch (type) {
  case fluid_type::fresh_water:
    return os << "fresh_water";
  case fluid_type::fuel:
    return os << "fuel";
  case fluid_type::waste_water:
    return os << "waste_water";
  case fluid_type::unknown:
    return os << "unknown";
  default:
    return os << "invalid";
  }
}

std::ostream& operator<<(std::ostream& os, const tank_device& device) {
  return os << "tank_device{name=" << device.name << ",type=" << device.type
            << ",capacity=" << device.capacity
            << ",volume_sensor=" << device.volume_sensor
            << ",level_sensor=" << device.level_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const battery_type type) {
  switch (type) {
  case battery_type::wet_low_maintenance:
    return os << "wet_low_maintenance";
  case battery_type::wet_maintenance_free:
    return os << "wet_maintenance_free";
  case battery_type::agm:
    return os << "agm";
  case battery_type::deep_cycle:
    return os << "deep_cycle";
  case battery_type::gel:
    return os << "gel";
  case battery_type::lifepo4:
    return os << "lifepo4";
  case battery_type::unknown:
    return os << "unknown";
  default:
    return os << "invalid";
  }
}

std::ostream& operator<<(std::ostream& os, const battery_device& device) {
  return os << "battery_device{name=" << device.name << ",type=" << device.type
            << ",capacity=" << device.capacity
            << ",charge_sensor=" << device.charge_sensor
            << ",remaining_capacity_sensor=" << device.remaining_capacity_sensor
            << ",current_sensor=" << device.current_sensor
            << ",voltage=" << device.voltage_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const device& device) {
  std::visit([&](const auto& d) { os << d; }, device);
  return os;
}

} // namespace spymarine
