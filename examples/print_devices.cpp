#include "example_utils.hpp"

#include "spymarine/discover.hpp"
#include "spymarine/home_assistant.hpp"
#include "spymarine/read_devices.hpp"

#include <print>

int main(int argc, char** argv) {
  std::println("Discover Simarine device");

  const auto result = spymarine::discover()
                          .transform_error(spymarine::error_from_error_code)
                          .and_then([](const auto ip) {
                            std::println("Read devices");

                            return spymarine::read_devices(ip);
                          })
                          .transform([](auto devices) {
                            std::println("Found {} devices", devices.size());

                            for (const auto& device : devices) {
                              std::println("{}", device_string(device));
                            }
                          });

  if (!result) {
    std::println("Failed to read sensor states: {}",
                 spymarine::error_message(result.error()));
  }

  return 0;
}
