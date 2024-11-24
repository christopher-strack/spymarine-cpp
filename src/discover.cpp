#include "spymarine/discover.hpp"
#include "spymarine/udp_socket.hpp"

namespace spymarine {

std::expected<uint32_t, error> discover(uint16_t port) {
  return udp_socket::open().and_then([port](auto udp_socket) {
    return udp_socket.bind(0, port).and_then(
        [&]() { return udp_socket.discover(); });
  });
}

} // namespace spymarine
