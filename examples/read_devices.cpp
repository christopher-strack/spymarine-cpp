#include "spymarine/client.hpp"
#include "spymarine/defaults.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/tcp_socket.hpp"
#include "spymarine/udp_socket.hpp"

#include <arpa/inet.h>

#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
  const auto discovered_ip =
      spymarine::udp_socket::open().and_then([](auto udp_socket) {
        return udp_socket.bind(INADDR_ANY, spymarine::simarine_default_udp_port)
            .and_then([&]() { return udp_socket.discover(); });
      });

  if (discovered_ip) {
    std::cout << "Reading devices" << std::endl;
    if (const auto devices = spymarine::read_devices(*discovered_ip)) {
      std::cout << "Devices read" << std::endl;
      for (const auto& device : *devices) {
        std::stringstream str;
        str << device;
        std::cout << "Device: " << str.str() << std::endl;
      }
    }
  } else {
    std::cerr << "Couldn't find Simarine device: "
              << spymarine::error_message(discovered_ip.error()) << std::endl;
  }

  return 0;
}
