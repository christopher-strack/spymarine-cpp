#pragma once

#include "spymarine/file_descriptor.hpp"

#include <sys/socket.h>

#include <cstdint>
#include <expected>
#include <span>
#include <system_error>

namespace spymarine {

class udp_socket {
public:
  static std::expected<udp_socket, std::error_code> open();

  std::expected<void, std::error_code> bind(uint32_t ip, uint16_t port);

  std::expected<uint32_t, std::error_code> discover();

  std::expected<std::span<uint8_t>, std::error_code>
  receive(std::span<uint8_t> buffer);

  udp_socket(const udp_socket& rhs) = delete;
  udp_socket(udp_socket&& rhs) = default;

  udp_socket& operator=(const udp_socket& rhs) = delete;
  udp_socket& operator=(udp_socket&& rhs) = default;

private:
  udp_socket(file_descriptor fd);

  file_descriptor _fd;
};

} // namespace spymarine
