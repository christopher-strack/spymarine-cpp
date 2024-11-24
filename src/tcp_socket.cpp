#include "spymarine/tcp_socket.hpp"
#include "spymarine/file_descriptor.hpp"

#include <netinet/in.h>
#include <sys/socket.h>

namespace spymarine {

tcp_socket::tcp_socket(file_descriptor fd) : _fd{std::move(fd)} {}

std::expected<tcp_socket, error> tcp_socket::open() {
  auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    return std::unexpected{std::error_code{errno, std::system_category()}};
  }
  return tcp_socket{file_descriptor(fd)};
}

std::expected<void, error> tcp_socket::connect(uint32_t ip, uint16_t port) {
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(ip);
  address.sin_port = htons(port);

  if (::connect(_fd.get(), reinterpret_cast<sockaddr*>(&address),
                sizeof(address)) == -1) {
    return std::unexpected{std::error_code{errno, std::system_category()}};
  }
  return {};
}

std::expected<void, error> tcp_socket::send(std::span<uint8_t> buffer) {
  const auto result = ::send(_fd.get(), buffer.data(), buffer.size(), 0);
  if (result == -1) {
    return std::unexpected{std::error_code{errno, std::system_category()}};
  }
  return {};
}

std::expected<std::span<const uint8_t>, error>
tcp_socket::receive(std::span<uint8_t> buffer) {
  const auto result = ::recv(_fd.get(), buffer.data(), buffer.size(), 0);
  if (result == -1) {
    return std::unexpected{std::error_code{errno, std::system_category()}};
  }

  if (result == 0) {
    return std::span<uint8_t>{};
  }

  return std::span<uint8_t>{buffer.data(), size_t(result)};
}

} // namespace spymarine
