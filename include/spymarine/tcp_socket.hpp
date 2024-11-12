#pragma once

#include "spymarine/file_descriptor.hpp"

#include <sys/socket.h>

#include <cstdint>
#include <optional>
#include <span>

namespace spymarine {

class tcp_socket {
public:
  tcp_socket(uint32_t ip, uint16_t port);

  tcp_socket(const tcp_socket& rhs) = delete;
  tcp_socket(tcp_socket&& rhs) = default;

  tcp_socket& operator=(const tcp_socket& rhs) = delete;
  tcp_socket& operator=(tcp_socket&& rhs) = default;

  bool send(std::span<uint8_t>);

  std::optional<std::span<uint8_t>> receive(std::span<uint8_t> buffer);

private:
  file_descriptor _socket;
};

} // namespace spymarine
