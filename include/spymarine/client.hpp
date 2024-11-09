#pragma once

#include "parsing.hpp"

#include <concepts>

namespace spymarine {

template <typename tcp_socket_type>
concept tcp_socket = requires(tcp_socket_type socket) {
  { socket.send(std::span<uint8_t>{}) } -> std::same_as<bool>;
  {
    socket.receive(std::span<uint8_t>{})
  } -> std::same_as<std::optional<std::span<uint8_t>>>;
};

template <tcp_socket tcp_socket_type> class client {
public:
  client(tcp_socket_type socket) : _socket{std::move(socket)} {}

  std::optional<message> request(const std::span<uint8_t>& data) {
    if (_socket.send(data)) {
      if (const auto raw_response = _socket.receive(_receive_buffer)) {
        return parse_message(*raw_response);
      }
    }
    return std::nullopt;
  }

private:
  tcp_socket_type _socket;
  std::array<uint8_t, 1024> _receive_buffer;
};

} // namespace spymarine
