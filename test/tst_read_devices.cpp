#include "data.hpp"
#include "raw_data.hpp"

#include "spymarine/buffer.hpp"
#include "spymarine/device.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/read_devices.hpp"

#include <catch2/catch_all.hpp>

#include <expected>
#include <ranges>
#include <system_error>

namespace spymarine {
namespace {
template <typename T> class test_tcp_socket_base {
public:
  static std::expected<T, error> open() { return T{}; }

  std::expected<void, error> connect(uint32_t, uint16_t) { return {}; }
};

class mock_tcp_socket : public test_tcp_socket_base<mock_tcp_socket> {
public:
  std::expected<void, error> send(std::span<uint8_t> data) {
    _last_sent_message = parse_message(data);
    return {};
  }

  std::expected<std::span<const uint8_t>, error>
  receive(std::span<uint8_t> buffer) {
    if (!_last_sent_message) {
      return std::unexpected{std::make_error_code(std::errc::io_error)};
    }

    if (_last_sent_message->type == message_type::device_count) {
      std::ranges::copy(raw_device_count_response, buffer.begin());
      return buffer.subspan(0, raw_device_count_response.size());
    } else if (_last_sent_message->type == message_type::device_info) {
      const auto device_id = _last_sent_message->data[5];
      const auto& data = raw_device_info_response[device_id];
      std::ranges::copy(data, buffer.begin());
      return buffer.subspan(0, data.size());
    }

    return std::unexpected{std::make_error_code(std::errc::io_error)};
  }

private:
  std::expected<message, error> _last_sent_message;
};

class failing_tcp_socket : public test_tcp_socket_base<failing_tcp_socket> {
public:
  std::expected<void, error> send(std::span<uint8_t> data) { return {}; }

  std::expected<std::span<const uint8_t>, error>
  receive(std::span<uint8_t> buffer) {
    return std::unexpected{std::make_error_code(std::errc::connection_refused)};
  }
};
} // namespace

TEST_CASE("read_devices") {
  buffer buffer;

  SECTION("return parsed devices") {
    const auto devices =
        read_devices<mock_tcp_socket>(buffer, 0, 0, do_not_filter_devices{});

    CHECK(devices == parsed_devices);
  }

  SECTION("return connection error") {
    const auto devices =
        read_devices<failing_tcp_socket>(buffer, 0, 0, do_not_filter_devices{});

    REQUIRE_FALSE(devices);
    CHECK(std::holds_alternative<std::error_code>(devices.error()));
  }
}

} // namespace spymarine
