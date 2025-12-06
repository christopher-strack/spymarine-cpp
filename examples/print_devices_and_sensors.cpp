#include "spymarine/client.hpp"
#include "spymarine/hub.hpp"
#include "spymarine/overloaded.hpp"

#include <print>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  std::print("Discover and connect to Simarine... ");
  auto hub = spymarine::discover_and_connect().and_then(
      spymarine::initialize_real_hub);
  if (!hub) {
    std::println("failed: {}", spymarine::error_message(hub.error()));
    return 1;
  }
  std::println("done");

  std::print("Updating sensor values... ");
  const auto update_result = hub->update_sensor_values();
  if (!update_result) {
    std::println("failed: {}", spymarine::error_message(update_result.error()));
    return 1;
  }
  std::println("done.");

  for (const auto& device_ : hub->devices()) {
    std::println("Device {}: {}", get_device_id(device_),
                 get_device_name(device_).value_or("<unnamed>"));
  }

  for (const auto& sensor_ : hub->sensors()) {
    std::visit(spymarine::overloaded{
                   [](const spymarine::battery_sensor2& s) {
                     std::println("Battery Sensor {}: {} %", s.id,
                                  s.charge.current_value.to_float());
                   },
                   [](const spymarine::tank_sensor2& s) {
                     std::println("Tank Sensor {}: {} %", s.id,
                                  s.level.current_value.to_float());
                   },
                   [](const spymarine::temperature_sensor2& s) {
                     std::println("Temperature Sensor {}: {} °C", s.id,
                                  s.value.current_value.to_float());
                   },
                   [](const auto& s) {
                     std::println("Sensor {}: {}", s.id,
                                  s.value.current_value.to_float());
                   },
                   [](const spymarine::unsupported_sensor2& s) {
                     std::println("Unsupported Sensor ({}) {}: {}", s.raw_type,
                                  s.id, s.raw_value);
                   },
               },
               sensor_);
  }

  return 0;
}
