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
    client<mock_tcp_socket> client{0, 0, std::chrono::seconds{0}};
    std::vector<device> devices;
    REQUIRE(client.read_devices(devices));

    const auto expected_devices = std::vector<device>{
        barometer_device{"Barometer", 3},
        pico_internal_device{5},
        voltage_device{"ST107 [5596] 1", 11},
        voltage_device{"ST107 [5596] 2", 12},
        voltage_device{"ST107 [5596] 3", 13},
        resistive_device{"ST107 [5596] 1", 14},
        resistive_device{"ST107 [5596] 2", 15},
        resistive_device{"ST107 [5596] 3", 16},
        resistive_device{"ST107 [5596] 4", 17},
        current_device{"SC303 [5499]", 19},
        voltage_device{"SC303 [5499] 1", 21},
        voltage_device{"SC303 [5499] 2", 22},
        resistive_device{"SC303 [5499] 1", 23},
        resistive_device{"SC303 [5499] 2", 24},
        resistive_device{"SC303 [5499] 3", 25},
        battery_device{"Bulltron", battery_type::lifepo4, 300.0f, 26},
        temperature_device{"Batterie", 31},
        tank_device{"Frischwasser", fluid_type::fresh_water, 100.0f, 32},
        battery_device{"Starterbatterie", battery_type::agm, 100.0f, 33},
        tank_device{"Abwasser", fluid_type::waste_water, 70.0f, 38},
        temperature_device{"Innen", 39},
        temperature_device{"Au\xa3\x65n ", 40},
        temperature_device{"Boiler", 41},
    };
    CHECK(devices == expected_devices);
  }

  BENCHMARK("read_devices") {
    client<mock_tcp_socket> client{0, 0, std::chrono::seconds{0}};
    std::vector<device> devices;
    [[maybe_unused]] const auto result = client.read_devices(devices);
    return devices;
  };
}

} // namespace spymarine
