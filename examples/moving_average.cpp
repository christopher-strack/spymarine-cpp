#include "spymarine/client.hpp"
#include "spymarine/hub.hpp"
#include "spymarine/sensor2.hpp"

#include <chrono>
#include <print>

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

  const auto update_interval = std::chrono::seconds(10);
  auto last_update_time = std::optional<clock::time_point>{};
  auto current_sensors =
      hub->sensors() | std::views::filter([](const auto& s) {
        return std::holds_alternative<spymarine::current_sensor2>(s);
      }) |
      std::views::transform(
          [](const auto& s) -> const spymarine::current_sensor2& {
            return std::get<spymarine::current_sensor2>(s);
          });

  // print the moving average of current sensors every update_interval
  while (true) {
    hub->read_sensor_values();

    const auto last_update_time_elapsed =
        last_update_time
            .transform([=](auto& t) {
              const auto elapsed = clock::now() - t;
              return elapsed >= update_interval;
            })
            .value_or(true);

    if (last_update_time_elapsed) {
      for (const auto& sensor_ : current_sensors) {
        std::println("Sensor #{}: {} A", sensor_.id,
                     sensor_.value.average_value);
      }

      hub->start_new_average_window();
      last_update_time = std::chrono::steady_clock::now();
    }
  }

  return 0;
}
