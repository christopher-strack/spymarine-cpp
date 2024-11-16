#include "spymarine/client.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/tcp_socket.hpp"
#include "spymarine/udp_socket.hpp"

#include <arpa/inet.h>

#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
  constexpr auto simarine_default_udp_port = 43210;
  constexpr auto simarine_default_tcp_port = 5001;

  const auto discovered_ip =
      spymarine::udp_socket::open()
          .and_then([](auto udp_socket) {
            return udp_socket.bind(INADDR_ANY, simarine_default_udp_port)
                .transform([&]() { return std::move(udp_socket); });
          })
          .and_then([](auto udp_socket) { return udp_socket.discover(); });

  if (discovered_ip) {
    spymarine::client<spymarine::tcp_socket> client{*discovered_ip,
                                                    simarine_default_tcp_port};

    std::cout << "Reading devices" << std::endl;
    std::vector<spymarine::device> devices;
    if (client.read_devices(devices)) {
      std::cout << "Devices read" << std::endl;
      for (const auto& device : devices) {
        std::stringstream str;
        str << device;
        std::cout << "Device: " << str.str() << std::endl;
      }
    }
  }

  return 0;
}
