#include "data.hpp"
#include "raw_data.hpp"

#include "spymarine/client.hpp"
#include "spymarine/device.hpp"
#include "spymarine/device_ostream.hpp"

#include <catch2/catch_all.hpp>

#include <expected>
#include <ranges>
#include <system_error>

namespace spymarine {
namespace {
class mock_tcp_socket {
public:
  static std::expected<mock_tcp_socket, std::error_code> open() {
    return mock_tcp_socket{};
  }

  std::expected<void, std::error_code> connect(uint32_t, uint16_t) {
    return {};
  }

  std::expected<void, std::error_code> send(std::span<uint8_t> data) {
    _last_sent_message = parse_message(data);
    return {};
  }

  std::expected<std::span<uint8_t>, std::error_code>
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
} // namespace

TEST_CASE("client") {
  SECTION("read_devices") {
    const auto devices =
        read_devices<mock_tcp_socket>(0, 0, std::chrono::seconds{0});

    CHECK(devices == parsed_devices);
  }
}

} // namespace spymarine
