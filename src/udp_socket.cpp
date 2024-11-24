#include "spymarine/udp_socket.hpp"
#include "spymarine/file_descriptor.hpp"

#include <netinet/in.h>
#include <sys/socket.h>

namespace spymarine {

udp_socket::udp_socket(file_descriptor fd) : _fd{std::move(fd)} {}

std::expected<udp_socket, error> udp_socket::open() {
  auto fd = ::socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (fd == -1) {
    return std::unexpected{std::error_code{errno, std::system_category()}};
  }
  return udp_socket{file_descriptor(fd)};
}

std::expected<void, error> udp_socket::bind(uint32_t ip, uint16_t port) {
  sockaddr_in address{};
  address.sin_family = PF_INET;
  address.sin_addr.s_addr = htonl(ip);
  address.sin_port = htons(port);

  if (::bind(_fd.get(), reinterpret_cast<sockaddr*>(&address),
             sizeof(address)) == -1) {
    return std::unexpected{std::error_code{errno, std::system_category()}};
  }
  return {};
}

std::expected<uint32_t, error> udp_socket::discover() {
  sockaddr_in address;
  socklen_t address_len = sizeof(address);

  std::array<uint8_t, 1> buffer{};

  const auto result =
      ::recvfrom(_fd.get(), buffer.data(), buffer.size(), MSG_PEEK,
                 reinterpret_cast<sockaddr*>(&address), &address_len);
  if (result == -1) {
    return std::unexpected{std::error_code{errno, std::system_category()}};
  }

  return ntohl(address.sin_addr.s_addr);
}

std::expected<std::span<const uint8_t>, error>
udp_socket::receive(std::span<uint8_t> buffer) {
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
