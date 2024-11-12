#include "spymarine/client.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/tcp_socket.hpp"

#include <arpa/inet.h>

#include <iostream>
#include <sstream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <IP>" << std::endl;
    return 1;
  }

  const char* ip_str = argv[1];

  uint32_t ip;
  if (inet_pton(AF_INET, ip_str, &ip) != 1) {
    std::cerr << "Invalid IP address format: " << ip_str << std::endl;
    return 1;
  }

  constexpr auto simarine_default_tcp_port = 5001;
  spymarine::client<spymarine::tcp_socket> client{ip,
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

  return 0;
}
