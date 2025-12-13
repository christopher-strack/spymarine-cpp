#include "spymarine/client.hpp"
#include "spymarine/device2.hpp"
#include "spymarine/home_assistant2.hpp"
#include "spymarine/hub.hpp"

#include <print>

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

  for (const spymarine::device2& device_ : hub->supported_devices()) {
    std::println("Device #{}", spymarine::get_device_id(device_));

    const auto discovery_msg =
        make_home_assistant_device_discovery_message(device_, *hub);
    std::println("Discovery Topic: {}", discovery_msg.topic);
    std::println("Discovery Payload: {}", discovery_msg.payload);

    const auto config = spymarine::home_assistant_state_config{};
    const auto state_msg =
        make_home_assistant_state_message(device_, *hub, config);
    std::println("State Topic: {}", state_msg.topic);
    std::println("State Payload: {}", state_msg.payload);
    std::println();
  };

  return 0;
}
