#include "spymarine/client.hpp"
#include "spymarine/device2.hpp"
#include "spymarine/hub.hpp"
#include "spymarine/overloaded.hpp"

#include <print>

namespace {

constexpr std::string
sensor_current_value_string(const spymarine::sensor2& sensor_) {
  return std::visit(
      spymarine::overloaded{
          [](const spymarine::voltage_sensor2& s) {
            return std::format("{} V", s.value.current_value.to_string());
          },
          [](const spymarine::current_sensor2& s) {
            return std::format("{} A", s.value.current_value.to_string());
          },
          [](const spymarine::temperature_sensor2& s) {
            return std::format("{} °C", s.value.current_value.to_string());
          },
          [](const spymarine::barometer_sensor2& s) {
            return std::format("{} mbar", s.value.current_value.to_string());
          },
          [](const spymarine::barometer_trend_sensor2& s) {
            return std::format("{} mbar/h", s.value.current_value.to_string());
          },
          [](const spymarine::resistive_sensor2& s) {
            return std::format("{} Ω", s.value.current_value.to_string());
          },
          [](const spymarine::tank_sensor2& s) {
            return std::format("Volume: {} L, Level: {} %",
                               s.volume.current_value.to_string(),
                               s.level.current_value.to_string());
          },
          [](const spymarine::battery_sensor2& s) {
            return std::format("Charge: {} %, Remaining Capacity: {} Ah",
                               s.charge.current_value.to_string(),
                               s.remaining_capacity.current_value.to_string());
          },
          [](const spymarine::unsupported_sensor2& s) {
            return std::format("Type {}, Raw Value: {}", s.raw_type,
                               s.raw_value);
          }},
      sensor_);
}

} // namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  std::print("Discovering Simarine system... ");
  const auto ip = spymarine::discover();
  if (!ip) {
    std::println("failed: {}", spymarine::error_message(ip.error()));
    return 1;
  }
  std::println("done");

  std::print("Connecting... ");
  auto hub = spymarine::connect(*ip).and_then(spymarine::initialize_hub);
  if (!hub) {
    std::println("failed: {}", spymarine::error_message(hub.error()));
    return 1;
  }
  std::println("done");

  std::print("Updating sensor values... ");
  const auto update_result = hub->read_sensor_values();
  if (!update_result) {
    std::println("failed: {}", spymarine::error_message(update_result.error()));
    return 1;
  }
  std::println("done");

  for (const spymarine::device2& device_ : hub->devices()) {
    std::println("Device #{}: {}", get_device_id(device_),
                 get_device_name(device_).value_or("<unnamed>"));

    for (const auto sensor : hub->sensors(device_)) {
      std::println("  Sensor #{}: {}", get_sensor_id(sensor),
                   sensor_current_value_string(sensor));
    }
  }

  return 0;
}
