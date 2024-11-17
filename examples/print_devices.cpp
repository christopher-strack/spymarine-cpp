#include "spymarine/device_ostream.hpp"
#include "spymarine/discover.hpp"
#include "spymarine/read_devices.hpp"

#include <print>
#include <sstream>

namespace {

std::string device_string(const spymarine::device& device) {
  std::stringstream str;
  str << device;
  return str.str();
}

} // namespace

int main(int argc, char** argv) {
  std::println("Discovering Simarine device");

  if (const auto ip = spymarine::discover()) {
    std::println("Reading devices");

    if (const auto devices = spymarine::read_devices(*ip)) {
      std::println("Found {} devices", devices->size());

      for (const auto& device : *devices) {
        std::println("Device: {}", device_string(device));
      }
    } else {
      std::println("Failed to read devices: {}",
                   spymarine::error_message(devices.error()));
    }
  } else {
    std::println("Couldn't find Simarine device: {}",
                 spymarine::error_message(ip.error()));
  }

  return 0;
}
