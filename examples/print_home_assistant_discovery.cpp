#include "spymarine/buffer.hpp"
#include "spymarine/discover.hpp"
#include "spymarine/home_assistant.hpp"
#include "spymarine/read_devices.hpp"

#include <print>

int main(int argc, char** argv) {
  std::println("Discover Simarine device");

  spymarine::buffer buffer;
  const auto result =
      spymarine::discover()
          .transform_error(spymarine::error_from_error_code)
          .and_then([&](const auto ip) {
            std::println("Read devices");

            return spymarine::read_devices(buffer, ip);
          })
          .transform([](auto devices) {
            std::println("Found {} devices", devices.size());

            for (const auto& device : devices) {
              if (const auto discovery_message =
                      spymarine::make_home_assistant_device_discovery_message(
                          device)) {
                std::println("{}", *discovery_message);
              }
            }
          });

  if (!result) {
    std::println("Failed to read sensor states: {}",
                 spymarine::error_message(result.error()));
  }

  return 0;
}
