#include "example_utils.hpp"

#include "spymarine/buffer.hpp"
#include "spymarine/discover.hpp"
#include "spymarine/read_devices.hpp"

#include <print>

int main(int argc, char** argv) {
  std::println("Discover Simarine device");

  spymarine::buffer buffer;
  const auto result = spymarine::discover()
                          .and_then([&](const auto ip) {
                            std::println("Read devices");

                            return spymarine::read_devices(buffer, ip);
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
