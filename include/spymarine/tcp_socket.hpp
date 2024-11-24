#pragma once

#include "spymarine/error.hpp"
#include "spymarine/file_descriptor.hpp"

#include <cstdint>
#include <expected>
#include <span>

namespace spymarine {

class tcp_socket {
public:
  static std::expected<tcp_socket, error> open();

  std::expected<void, error> connect(uint32_t ip, uint16_t port);

  std::expected<void, error> send(std::span<uint8_t>);

  std::expected<std::span<const uint8_t>, error>
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
