#include "spymarine/tcp_socket.hpp"
#include "spymarine/file_descriptor.hpp"

#include <netinet/in.h>

#include <cstring>

namespace spymarine {

namespace {

file_descriptor create_tcp_socket(uint32_t ip, uint16_t port) {
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = ip;
  address.sin_port = htons(port);

  auto socket_fd = file_descriptor{::socket(AF_INET, SOCK_STREAM, 0)};
  if (!socket_fd.valid()) {
    return file_descriptor{};
  }
  if (::connect(socket_fd.get(), reinterpret_cast<sockaddr*>(&address),
                sizeof(address)) == -1) {
    return file_descriptor{};
  }
  return socket_fd;
}

} // namespace

tcp_socket::tcp_socket(uint32_t ip, uint16_t port)
    : _socket{create_tcp_socket(ip, port)} {}

bool tcp_socket::send(std::span<uint8_t> buffer) {
  const auto result = ::send(_socket.get(), buffer.data(), buffer.size(), 0);
  if (result == -1) {
    return false;
  }
  return true;
}

std::optional<std::span<uint8_t>>
tcp_socket::receive(std::span<uint8_t> buffer) {
  const auto result = ::recv(_socket.get(), buffer.data(), buffer.size(), 0);
  if (result == -1) {
    return std::nullopt;
  }

  if (result == 0) {
    _socket = file_descriptor{};
    return std::nullopt;
  }

  return std::span<uint8_t>{buffer.data(), size_t(result)};
}

} // namespace spymarine
