#pragma once

#include "parsing.hpp"

#include <concepts>
#include <optional>
#include <variant>

namespace spymarine {

template <typename tcp_socket_type>
concept tcp_socket = requires(tcp_socket_type socket) {
  { socket.send(std::span<uint8_t>{}) } -> std::same_as<bool>;
  {
    socket.receive(std::span<uint8_t>{})
  } -> std::same_as<std::optional<std::span<uint8_t>>>;
};

std::span<uint8_t> write_data(message m, std::span<uint8_t> buffer);

struct device_count_request {
  std::span<uint8_t> write_message_data(std::span<uint8_t> buffer) const;
};

struct device_info_request {
  uint8_t device_id;

  std::span<uint8_t> write_message_data(std::span<uint8_t> buffer) const;
};

using request = std::variant<device_count_request, device_info_request>;

template <tcp_socket tcp_socket_type> class client {
public:
  client(tcp_socket_type socket) : _socket{std::move(socket)} {}

  std::optional<message> request(const request& request) {
    const auto data = std::visit(
        [this](const auto& r) { return r.write_message_data(_buffer); },
        request);

    if (_socket.send(data)) {
      if (const auto raw_response = _socket.receive(_buffer)) {
        return parse_message(*raw_response);
      }
    }
    return std::nullopt;
  }

private:
  tcp_socket_type _socket;
  std::array<uint8_t, 1024> _buffer;
};

} // namespace spymarine
