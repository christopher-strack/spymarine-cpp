#include "catch_string_maker.hpp"
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
  std::expected<void, error> send(std::span<uint8_t> bytes) {
    if (const auto message = parse_message(bytes)) {
      if (message->type == message_type::device_count) {
        _response = raw_device_count_response;
      } else if (message->type == message_type::device_info) {
        const auto device_id = message->data[5];
        _response = raw_device_info_response[device_id];
      } else {
        return std::unexpected{
            std::make_error_code(std::errc::connection_refused)};
      }
    }
    return {};
  }

  std::expected<std::span<const uint8_t>, error>
  receive(std::span<uint8_t> buffer) {
    const auto length = std::min(buffer.size(), _response.size());
    std::ranges::copy_n(_response.begin(), length, buffer.begin());
    _response.erase(_response.begin(), _response.begin() + length);
    return buffer.subspan(0, length);
  }

private:
  std::vector<uint8_t> _response;
};

class failing_tcp_socket : public test_tcp_socket_base<failing_tcp_socket> {
public:
  std::expected<void, error> send(std::span<uint8_t> bytes) { return {}; }

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
