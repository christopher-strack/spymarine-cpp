#pragma once

#include "spymarine/defaults.hpp"
#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/overloaded.hpp"
#include "spymarine/parse_device.hpp"
#include "spymarine/parse_error.hpp"
#include "spymarine/parse_message.hpp"
#include "spymarine/tcp_socket.hpp"

#include <array>
#include <concepts>
#include <cstdint>
#include <expected>
#include <functional>

namespace spymarine {

struct do_not_filter_devices {
  bool operator()(const device& devices) { return true; }
};

template <typename... DeviceTypes> struct filter_by_device_type {
  bool operator()(const device& devices) {
    return (std::holds_alternative<DeviceTypes>(devices) || ...);
  }
};

} // namespace spymarine

namespace spymarine::detail {

template <typename tcp_socket_type>
concept tcp_socket_concept =
    requires(tcp_socket_type socket, uint32_t ip, uint16_t port) {
      {
        tcp_socket_type::open()
      } -> std::same_as<std::expected<tcp_socket_type, error>>;
      { socket.connect(ip, port) } -> std::same_as<std::expected<void, error>>;
      {
        socket.send(std::span<uint8_t>{})
      } -> std::same_as<std::expected<void, error>>;
      {
        socket.receive(std::span<uint8_t>{})
      } -> std::same_as<std::expected<std::span<const uint8_t>, error>>;
    };

std::span<uint8_t> write_message_data(message m, std::span<uint8_t> buffer);

template <tcp_socket_concept tcp_socket_type> class device_reader {
public:
  device_reader(std::span<uint8_t> buffer, tcp_socket_type socket,
                std::invocable<const device&> auto filter_function)
      : _buffer{buffer}, _socket{std::move(socket)},
        _filter_function{std::move(filter_function)} {}

  std::expected<std::vector<device>, error> read_devices() {
    return request_device_count().and_then(
        [&](const auto device_count) { return read_devices(device_count); });
  }

private:
  std::expected<std::vector<device>, error>
  read_devices(const uint8_t device_count) {
    std::vector<device> devices;
    devices.reserve(device_count);

    uint8_t state_start_index = 0;

    for (uint8_t i = 0; i < device_count; i++) {
      if (auto parsed_device = request_device_info(i, state_start_index)) {
        state_start_index += sensor_state_offset(*parsed_device);

        std::visit(overloaded{
                       [](null_device) {},
                       [](unknown_device) {},
                       [this, &devices](auto device) {
                         if (_filter_function(device)) {
                           devices.insert(devices.end(), std::move(device));
                         }
                       },
                   },
                   std::move(*parsed_device));
      } else {
        return std::unexpected{parsed_device.error()};
      }
    }

    return devices;
  }

  std::expected<uint8_t, error> request_device_count() {
    return request_message({message_type::device_count, {}})
        .and_then([](const auto& message) -> std::expected<uint8_t, error> {
          if (message.type == message_type::device_count) {
            message_values_view values{message.data};
            if (const auto count =
                    find_value_for_type<numeric_value>(1, values)) {
              return count->number() + 1;
            }
          }
          return std::unexpected{parse_error::invalid_device_count_message};
        });
  }

  std::expected<parsed_device, error>
  request_device_info(uint8_t device_id, uint8_t state_start_index) {
    const std::array<uint8_t, 19> data{
        0x00, 0x01, 0x00, 0x00, 0x00, device_id, 0xff, 0x01, 0x03, 0x00,
        0x00, 0x00, 0x00, 0xff, 0x00, 0x00,      0x00, 0x00, 0xff};

    return request_message({message_type::device_info, data})
        .and_then([state_start_index](const auto& message)
                      -> std::expected<parsed_device, error> {
          if (message.type == message_type::device_info) {
            return parse_device(message.data, state_start_index);
          } else {
            return std::unexpected{parse_error::invalid_device_message};
          }
        });
  }

  std::expected<message, error> request_message(const message& msg) {
    return _socket.send(detail::write_message_data(msg, _buffer))
        .and_then([&, this]() { return _socket.receive(_buffer); })
        .and_then([](const std::span<const uint8_t> data) {
          return parse_message(data);
        });
  }

  std::span<uint8_t> _buffer;
  tcp_socket_type _socket;
  std::function<bool(const device&)> _filter_function;
};

} // namespace spymarine::detail

namespace spymarine {

template <detail::tcp_socket_concept tcp_socket_type = tcp_socket>
std::expected<std::vector<device>, error>
read_devices(std::span<uint8_t> buffer, const uint32_t address,
             const uint16_t port = simarine_default_tcp_port,
             std::function<bool(const device&)> filter_function =
                 do_not_filter_devices{}) {
  return tcp_socket_type::open().and_then([&](auto socket) {
    return socket.connect(address, port).and_then([&]() {
      detail::device_reader<tcp_socket_type> device_reader{
          buffer, std::move(socket), std::move(filter_function)};
      return device_reader.read_devices();
    });
  });
}

} // namespace spymarine
