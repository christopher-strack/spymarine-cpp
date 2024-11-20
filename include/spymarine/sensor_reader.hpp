#pragma once

#include "spymarine/device.hpp"
#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/parse_message.hpp"
#include "spymarine/udp_socket.hpp"

#include <expected>
#include <system_error>
#include <unordered_map>
#include <vector>

namespace spymarine {

using sensor_map = std::unordered_map<uint8_t, std::vector<sensor*>>;

sensor_map build_sensor_map(std::vector<device>& devices_range);

template <typename udp_socket_type>
concept udp_socket_concept = requires(udp_socket_type socket, uint32_t ip,
                                      uint16_t port) {
  {
    socket.receive(std::span<uint8_t>{})
  } -> std::same_as<std::expected<std::span<const uint8_t>, std::error_code>>;
};

enum class update_method { replace, cumulative_average };

template <udp_socket_concept udp_socket_type> class sensor_reader {
public:
  sensor_reader(std::vector<device>& devices, udp_socket_type udp_socket)
      : _udp_socket{std::move(udp_socket)} {
    _sensor_map = build_sensor_map(devices);
  }

  std::expected<void, error> read_and_update(update_method method) {
    switch (method) {
    case update_method::replace:
      _average_count = 0;
      return read_and_update([](float, float new_value) { return new_value; });
    case update_method::cumulative_average:
      return read_and_update([this](float last_value, float new_value) {
        const auto result =
            (last_value * _average_count + new_value) / (_average_count + 1);
        _average_count++;
        return result;
      });
    }
  }

private:
  std::expected<void, error>
  read_and_update(std::invocable<float, float> auto update_function) {
    return _udp_socket.receive(_buffer)
        .transform_error(error_from_error_code)
        .and_then([](const auto data) {
          return parse_message(data).transform_error(error_from_parse_error);
        })
        .transform([this, &update_function](const auto message) {
          read_and_update(message, _sensor_map, update_function);
        });
  }

  void read_and_update(message state_message, sensor_map& map,
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

  udp_socket_type _udp_socket;
  sensor_map _sensor_map;
  std::array<uint8_t, 1024> _buffer;
  int _average_count = 0;
};

std::expected<sensor_reader<udp_socket>, error>
make_sensor_reader(std::vector<device>& devices);

} // namespace spymarine
