#pragma once

#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/parse_message.hpp"
#include "spymarine/udp_socket.hpp"

#include <chrono>
#include <expected>
#include <optional>
#include <unordered_map>
#include <vector>

namespace spymarine {

using sensor_map = std::unordered_map<uint8_t, std::vector<sensor*>>;

sensor_map build_sensor_map(std::vector<device>& devices_range);

template <typename udp_socket_type>
concept udp_socket_concept =
    requires(udp_socket_type socket, uint32_t ip, uint16_t port) {
      {
        socket.receive(std::span<uint8_t>{})
      } -> std::same_as<std::expected<std::span<const uint8_t>, error>>;
    };

template <udp_socket_concept udp_socket_type> class sensor_reader_base {
public:
  sensor_reader_base(std::span<uint8_t> buffer, std::vector<device>& devices,
                     udp_socket_type udp_socket)
      : _udp_socket{std::move(udp_socket)}, _buffer{buffer} {
    _sensor_map = build_sensor_map(devices);
  }

protected:
  std::expected<void, error>
  read_and_process_values(std::invocable<float, float> auto update_function) {
    std::expected<void, error> result =
        std::unexpected{parse_error::unknown_message};

    while (!result.has_value() &&
           result.error() == error{parse_error::unknown_message}) {
      result =
          _udp_socket.receive(_buffer)
              .and_then([](const auto bytes) { return parse_message(bytes); })
              .transform([this, &update_function](const auto message) {
                read_and_process_values(message, _sensor_map, update_function);
              });
    }

    return result;
  }

  void
  read_and_process_values(message state_message, sensor_map& map,
                          std::invocable<float, float> auto update_function) {
    if (state_message.type != message_type::sensor_state) {
      return;
    }

    message_values_view state_values{state_message.data};

    for (const auto& entry : state_values) {
      if (const auto value = std::get_if<numeric_value>(&entry.value)) {
        if (const auto it = map.find(entry.id); it != map.end()) {
          for (sensor* sensor : it->second) {
            const auto new_value = sensor_value(*value, sensor->type);
            sensor->value = update_function(sensor->value, new_value);
          }
        }
      }
    }
  }

private:
  udp_socket_type _udp_socket;
  sensor_map _sensor_map;
  std::span<uint8_t> _buffer;
};

template <udp_socket_concept udp_socket_type>
class sensor_reader : public sensor_reader_base<udp_socket_type> {
public:
  using sensor_reader_base<udp_socket_type>::sensor_reader_base;

  std::expected<void, error> read_and_update() {
    return this->read_and_process_values(
        [](float, float new_value) { return new_value; });
  }
};

template <udp_socket_concept udp_socket_type>
class moving_average_sensor_reader
    : public sensor_reader_base<udp_socket_type> {
public:
  moving_average_sensor_reader(
      std::span<uint8_t> buffer,
      const std::chrono::steady_clock::duration moving_average_interval,
      std::vector<device>& devices, udp_socket_type udp_socket)
      : sensor_reader_base<udp_socket_type>(buffer, devices,
                                            std::move(udp_socket)),
        _moving_average_interval{moving_average_interval} {}

  using sensor_reader_base<udp_socket_type>::sensor_reader_base;

  std::expected<bool, error> read_and_update() {
    if (!_last_window_time.has_value()) {
      return this
          ->read_and_process_values(
              [](float, float new_value) { return new_value; })
          .transform([this]() {
            _average_count = 0;
            _last_window_time = std::chrono::steady_clock::now();
            return false;
          });
    }

    const auto delta = std::chrono::steady_clock::now() - *_last_window_time;
    const auto moving_average_window_completed =
        delta >= _moving_average_interval;

    if (moving_average_window_completed) {
      _last_window_time = std::nullopt;
      return true;
    } else {
      return this
          ->read_and_process_values([this](float last_value, float new_value) {
            return (last_value * _average_count + new_value) /
                   (_average_count + 1);
          })
          .transform([this]() {
            _average_count++;
            return false;
          });
    }
  }

private:
  std::chrono::steady_clock::duration _moving_average_interval;
  int _average_count = 0;
  std::optional<std::chrono::steady_clock::time_point> _last_window_time{};
};

std::expected<sensor_reader<udp_socket>, error>
make_sensor_reader(std::span<uint8_t> buffer, std::vector<device>& devices);

std::expected<moving_average_sensor_reader<udp_socket>, error>
make_moving_average_sensor_reader(
    std::span<uint8_t> buffer,
    std::chrono::steady_clock::duration moving_average_interval,
    std::vector<device>& devices);

} // namespace spymarine
