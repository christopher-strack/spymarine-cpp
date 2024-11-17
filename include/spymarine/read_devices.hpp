#pragma once

#include "spymarine/defaults.hpp"
#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/overloaded.hpp"
#include "spymarine/parse_device.hpp"
#include "spymarine/parse_message.hpp"
#include "spymarine/tcp_socket.hpp"

#include <array>
#include <chrono>
#include <concepts>
#include <cstdint>
#include <expected>
#include <optional>
#include <system_error>
#include <thread>
#include <variant>

namespace spymarine {

using read_devices_error = std::variant<error, std::error_code>;

inline read_devices_error to_client_error(const error& err) { return err; }

std::string error_message(read_devices_error error);

} // namespace spymarine

namespace spymarine::detail {

template <typename tcp_socket_type>
concept tcp_socket_concept =
    requires(tcp_socket_type socket, uint32_t ip, uint16_t port) {
      {
        tcp_socket_type::open()
      } -> std::same_as<std::expected<tcp_socket_type, std::error_code>>;
      {
        socket.connect(ip, port)
      } -> std::same_as<std::expected<void, std::error_code>>;
      {
        socket.send(std::span<uint8_t>{})
      } -> std::same_as<std::expected<void, std::error_code>>;
      {
        socket.receive(std::span<uint8_t>{})
      } -> std::same_as<std::expected<std::span<uint8_t>, std::error_code>>;
    };

std::span<uint8_t> write_message_data(message m, std::span<uint8_t> buffer);

template <tcp_socket_concept tcp_socket_type> class client {
public:
  client(uint32_t address, uint16_t port,
         const std::chrono::system_clock::duration request_limit =
             std::chrono::milliseconds{10})
      : _ip_address{address}, _port{port}, _request_limit{request_limit} {}

  std::expected<std::vector<device>, read_devices_error> read_devices() {
    std::vector<device> devices;
    request_device_count().and_then([&devices, this](const auto device_count) {
      return read_devices(device_count, devices);
    });
    return devices;
  }

private:
  std::expected<void, read_devices_error>
  read_devices(const uint8_t device_count, std::vector<device>& devices) {
    devices.reserve(device_count);

    uint8_t state_start_index = 0;

    for (uint8_t i = 0; i < device_count; i++) {
      if (auto parsed_device = request_device_info(i, state_start_index)) {
        state_start_index += sensor_state_offset(*parsed_device);

        std::visit(overloaded{
                       [](null_device) {},
                       [](unknown_device) {},
                       [&devices](auto device) {
                         devices.insert(devices.end(), std::move(device));
                       },
                   },
                   std::move(*parsed_device));
      } else {
        return std::unexpected{parsed_device.error()};
      }
    }

    return {};
  }

  std::expected<uint8_t, read_devices_error> request_device_count() {
    return request_message({message_type::device_count, {}})
        .and_then([](const auto& message)
                      -> std::expected<uint8_t, read_devices_error> {
          if (message.type == message_type::device_count &&
              message.data.size() >= 6) {
            return message.data[5] + 1;
          }
          return std::unexpected{error::invalid_device_count_message};
        });
  }

  std::expected<parsed_device, read_devices_error>
  request_device_info(uint8_t device_id, uint8_t state_start_index) {
    const std::array<uint8_t, 19> data{
        0x00, 0x01, 0x00, 0x00, 0x00, device_id, 0xff, 0x01, 0x03, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00,      0x00, 0x00, 0xff};

    return request_message({message_type::device_info, data})
        .and_then([state_start_index](const auto& message) {
          return parse_device(message.data, state_start_index)
              .transform_error(to_client_error);
        });
  }

  std::expected<message, read_devices_error>
  request_message(const message& msg) {
    wait_for_request_limit();

    const std::expected<std::span<uint8_t>, read_devices_error> raw_response =
        tcp_socket_type::open().and_then([&, this](auto socket) {
          return socket.connect(_ip_address, _port)
              .and_then([&, this]() {
                return socket.send(detail::write_message_data(msg, _buffer));
              })
              .and_then([&, this]() { return socket.receive(_buffer); });
        });

    return raw_response.and_then([](const auto raw_response) {
      return parse_message(raw_response).transform_error(to_client_error);
    });
  }

  void wait_for_request_limit() {
    if (_last_request_time) {
      const auto delta = std::chrono::system_clock::now() - *_last_request_time;
      if (delta < _request_limit) {
        std::this_thread::sleep_for(_request_limit - delta);
      }
    }
  }

  uint32_t _ip_address;
  uint16_t _port;
  std::array<uint8_t, 1024> _buffer;
  std::chrono::system_clock::duration _request_limit;
  std::optional<std::chrono::system_clock::time_point> _last_request_time;
};

} // namespace spymarine::detail

namespace spymarine {

template <typename tcp_socket_type = tcp_socket>
std::expected<std::vector<device>, read_devices_error>
read_devices(const uint32_t address,
             const uint16_t port = simarine_default_tcp_port,
             const std::chrono::system_clock::duration request_limit =
                 std::chrono::milliseconds{10}) {
  detail::client<tcp_socket_type> client{address, port, request_limit};
  return client.read_devices();
}

} // namespace spymarine