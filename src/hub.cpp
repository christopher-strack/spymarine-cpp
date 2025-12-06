#include "spymarine/hub.hpp"

namespace spymarine {

std::expected<hub<tcp_socket, udp_socket>, error>
initialize_hub(client<tcp_socket, udp_socket> client_) {
  return initialize_hub_with_sockets(std::move(client_));
}

} // namespace spymarine
