#include "spymarine/spymarine.hpp"

#include <print>

namespace {

constexpr std::string
sensor_current_value_string(const spymarine::sensor& sen) {
  return std::visit(
      spymarine::overloaded{
          [](const spymarine::voltage_sensor& s) {
            return std::format("{} V", s.value.current_value.to_string());
          },
          [](const spymarine::current_sensor& s) {
            return std::format("{} A", s.value.current_value.to_string());
          },
          [](const spymarine::temperature_sensor& s) {
            return std::format("{} °C", s.value.current_value.to_string());
          },
          [](const spymarine::barometer_sensor& s) {
            return std::format("{} mbar", s.value.current_value.to_string());
          },
          [](const spymarine::barometer_trend_sensor& s) {
            return std::format("{} mbar/h", s.value.current_value.to_string());
          },
          [](const spymarine::resistive_sensor& s) {
            return std::format("{} Ω", s.value.current_value.to_string());
          },
          [](const spymarine::tank_sensor& s) {
            return std::format("Volume: {} L, Level: {} %",
                               s.volume.current_value.to_string(),
                               s.level.current_value.to_string());
          },
          [](const spymarine::battery_sensor& s) {
            return std::format("Charge: {} %, Remaining Capacity: {} Ah",
                               s.charge.current_value.to_string(),
                               s.remaining_capacity.current_value.to_string());
          },
          [](const spymarine::unsupported_sensor& s) {
            return std::format("Type {}, Raw Value: {}", s.raw_type,
                               s.raw_value);
          }},
      sen);
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

  std::println("System Information");
  std::println("  Serial Number: {}", hub->system().serial_number);
  std::println("  Firmware Version: {}.{}", hub->system().fw_version.major,
               hub->system().fw_version.minor);

  for (const auto& dev : hub->all_devices()) {
    std::println("Device #{}: {}", get_device_id(dev),
                 get_device_name(dev).value_or("<unnamed>"));

    for (const auto& sen : hub->all_sensors(dev)) {
      std::println("  Sensor #{}: {}", get_sensor_id(sen),
                   sensor_current_value_string(sen));
    }
  }

  return 0;
}
