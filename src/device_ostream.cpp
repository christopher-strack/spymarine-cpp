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
  }

  return os << "invalid";
}

std::ostream& operator<<(std::ostream& os, const sensor& info) {
  return os << "sensor_info{type=" << info.type
            << ",state_index=" << int(info.state_index)
            << ",value=" << info.value << "}";
}

std::ostream& operator<<(std::ostream& os, const pico_internal_device& d) {
  return os << "pico_internal_device{voltage_sensor=" << d.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const voltage_device& d) {
  return os << "voltage_device{name=" << d.name
            << ",voltage_sensor=" << d.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const current_device& d) {
  return os << "current_device{name=" << d.name
            << ",current_sensor=" << d.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const temperature_device& d) {
  return os << "temperature_device{name=" << d.name
            << ",temperature_sensor=" << d.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const barometer_device& d) {
  return os << "barometer_device{name=" << d.name
            << ",pressure_sensor=" << d.device_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const resistive_device& d) {
  return os << "resistive_device{name=" << d.name
            << ",resistive_sensor=" << d.device_sensor << "}";
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
  }
  return os << "invalid";
}

std::ostream& operator<<(std::ostream& os, const tank_device& d) {
  return os << "tank_device{name=" << d.name << ",type=" << d.type
            << ",capacity=" << d.capacity
            << ",volume_sensor=" << d.volume_sensor
            << ",level_sensor=" << d.level_sensor << "}";
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
  }
  return os << "invalid";
}

std::ostream& operator<<(std::ostream& os, const battery_device& d) {
  return os << "battery_device{name=" << d.name << ",type=" << d.type
            << ",capacity=" << d.capacity
            << ",charge_sensor=" << d.charge_sensor
            << ",remaining_capacity_sensor=" << d.remaining_capacity_sensor
            << ",current_sensor=" << d.current_sensor
            << ",voltage=" << d.voltage_sensor << "}";
}

std::ostream& operator<<(std::ostream& os, const device& d_) {
  std::visit([&](const auto& d) { os << d; }, d_);
  return os;
}

} // namespace spymarine
