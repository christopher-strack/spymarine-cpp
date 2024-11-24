#pragma once

#include "spymarine/error.hpp"
#include "spymarine/file_descriptor.hpp"

#include <cstdint>
#include <expected>
#include <span>

namespace spymarine {

class udp_socket {
public:
  static std::expected<udp_socket, error> open();

  std::expected<void, error> bind(uint32_t ip, uint16_t port);

  std::expected<uint32_t, error> discover();

  std::expected<std::span<const uint8_t>, error>
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
