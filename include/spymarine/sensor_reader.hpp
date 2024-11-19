#pragma once

#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/parse_message.hpp"
#include "spymarine/udp_socket.hpp"

#include <expected>
#include <system_error>
#include <unordered_map>
#include <vector>

namespace spymarine {

using sensor_map = std::unordered_map<uint8_t, std::vector<sensor*>>;

sensor_map build_sensor_map(std::vector<device>& devices_range);

void update_sensor_states(message state_message, sensor_map& map);

template <typename udp_socket_type>
concept udp_socket_concept = requires(udp_socket_type socket, uint32_t ip,
                                      uint16_t port) {
  {
    socket.receive(std::span<uint8_t>{})
  } -> std::same_as<std::expected<std::span<const uint8_t>, std::error_code>>;
};

template <udp_socket_concept udp_socket_type> class sensor_reader {
public:
  sensor_reader(std::vector<device>& devices, udp_socket_type udp_socket)
      : _udp_socket{std::move(udp_socket)} {
    _sensor_map = build_sensor_map(devices);
  }

  std::expected<void, error> update() {
    return _udp_socket.receive(_buffer)
        .transform_error(error_from_error_code)
        .and_then([](const auto data) {
          return parse_message(data).transform_error(error_from_parse_error);
        })
        .transform([this](const auto message) {
          update_sensor_states(message, _sensor_map);
        });
  }

private:
  udp_socket_type _udp_socket;
  sensor_map _sensor_map;
  std::array<uint8_t, 1024> _buffer;
};

std::expected<sensor_reader<udp_socket>, error>
make_sensor_reader(std::vector<device>& devices);

} // namespace spymarine
