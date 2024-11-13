#pragma once

#include "spymarine/file_descriptor.hpp"

#include <sys/socket.h>

#include <cstdint>
#include <expected>
#include <span>
#include <system_error>

namespace spymarine {

class tcp_socket {
public:
  static std::expected<tcp_socket, std::error_code> open();

  std::expected<void, std::error_code> connect(uint32_t ip, uint16_t port);

  std::expected<void, std::error_code> send(std::span<uint8_t>);

  std::expected<std::span<uint8_t>, std::error_code>
  receive(std::span<uint8_t> buffer);

  tcp_socket(const tcp_socket& rhs) = delete;
  tcp_socket(tcp_socket&& rhs) = default;

  tcp_socket& operator=(const tcp_socket& rhs) = delete;
  tcp_socket& operator=(tcp_socket&& rhs) = default;

private:
  tcp_socket(file_descriptor fd);

  file_descriptor _fd;
};

} // namespace spymarine
