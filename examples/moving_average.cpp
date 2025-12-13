#include "spymarine/spymarine.hpp"

#include <chrono>
#include <print>

namespace {

void print_current_sensors(const spymarine::hub& hub) {
  for (const auto& current_sensor :
       hub.sensors_by_type<spymarine::current_sensor>()) {
    std::println("Sensor #{}: {} A", current_sensor.id,
                 current_sensor.value.average_value);
  }
}

} // namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  using clock = std::chrono::steady_clock;

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

  print_current_sensors(*hub);

  const auto update_interval = std::chrono::seconds(10);
  auto last_update_time = clock::now();

  // print the moving average of current sensors every update_interval
  while (true) {
    hub->read_sensor_values();

    if ((clock::now() - last_update_time) >= update_interval) {
      print_current_sensors(*hub);

      hub->start_new_average_window();
      last_update_time = clock::now();
    }
  }

  return 0;
}
