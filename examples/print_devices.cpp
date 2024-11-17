#include "spymarine/defaults.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/read_devices.hpp"
#include "spymarine/udp_socket.hpp"

#include <arpa/inet.h>

#include <iostream>
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

  const auto discovered_ip =
      spymarine::udp_socket::open().and_then([](auto udp_socket) {
        return udp_socket.bind(INADDR_ANY, spymarine::simarine_default_udp_port)
            .and_then([&]() { return udp_socket.discover(); });
      });

  if (discovered_ip) {
    std::println("Reading devices");

    if (const auto devices = spymarine::read_devices(*discovered_ip)) {
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
                 spymarine::error_message(discovered_ip.error()));
  }

  return 0;
}
