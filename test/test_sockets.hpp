#pragma once

#include "raw_data/count_info.hpp"
#include "raw_data/devices.hpp"
#include "raw_data/sensor_state.hpp"
#include "raw_data/sensors.hpp"

#include "spymarine/error.hpp"
#include "spymarine/message.hpp"
#include "spymarine/parse_message.hpp"

#include <cstdint>
#include <expected>
#include <span>
#include <vector>

namespace spymarine {

template <typename T> class test_tcp_socket_base {
public:
  static constexpr std::expected<T, error> open() noexcept { return T{}; }

  constexpr std::expected<void, error> connect(uint32_t, uint16_t) noexcept {
    return {};
  }
};

class mock_tcp_socket : public test_tcp_socket_base<mock_tcp_socket> {
public:
  constexpr std::expected<void, error> send(std::span<uint8_t> bytes) noexcept {
    using std::ranges::to;

    if (const auto message = parse_message(bytes)) {
      if (message->type() == message_type::count_information) {
        _response = raw_count_info_response | std::ranges::to<std::vector>();
      } else if (message->type() == message_type::device_information) {
        const auto value = message->values().find<numeric_value1>(0);
        assert(value.has_value());
        const auto id = size_t(value->int32());
        _response = raw_device_info_responses[id] | to<std::vector>();
      } else if (message->type() == message_type::sensor_information) {
        const auto value = message->values().find<numeric_value1>(1);
        assert(value.has_value());
        const auto id = value->uint32();
        _response = raw_sensor_info_responses[id] | to<std::vector>();
      } else {
        return std::unexpected{std::errc::connection_refused};
      }
    }
    return {};
  }

  constexpr std::expected<std::span<const uint8_t>, error>
  receive(std::span<uint8_t> buffer) noexcept {
    const auto length = std::min(buffer.size(), _response.size());
    const auto diff = static_cast<std::ptrdiff_t>(length);
    std::ranges::copy_n(_response.begin(), diff, buffer.begin());
    _response.erase(_response.begin(), _response.begin() + diff);
    return buffer.subspan(0, length);
  }

private:
  std::vector<uint8_t> _response;
};

class failing_tcp_socket : public test_tcp_socket_base<failing_tcp_socket> {
public:
  constexpr std::expected<void, error>
  send([[maybe_unused]] std::span<uint8_t> bytes) noexcept {
    return std::unexpected{std::errc::connection_refused};
  }

  constexpr std::expected<std::span<const uint8_t>, error>
  receive([[maybe_unused]] std::span<uint8_t> buffer) noexcept {
    return std::unexpected{std::errc::connection_refused};
  }
};

class mock_udp_socket {
public:
  std::expected<std::span<const uint8_t>, error>
  receive([[maybe_unused]] std::span<uint8_t> buffer) {
    return std::span<const uint8_t>{raw_state_response};
  }
};

class failing_udp_socket {
public:
  std::expected<std::span<const uint8_t>, error>
  receive([[maybe_unused]] std::span<uint8_t> buffer) {
    return std::unexpected{std::errc::io_error};
  }
};

} // namespace spymarine
