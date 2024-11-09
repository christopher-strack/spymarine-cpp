#include "raw_data.hpp"

#include "spymarine/client.hpp"
#include "spymarine/communication.hpp"
#include "spymarine/device.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/parsing.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {
namespace {
class mock_tcp_socket {
public:
  bool send(std::span<uint8_t> data) {
    _last_sent_message = parse_message(data);
    return true;
  }

  std::optional<std::span<uint8_t>> receive(std::span<uint8_t> buffer) {
    if (!_last_sent_message) {
      return std::nullopt;
    }

    if (_last_sent_message->type == message_type::device_count) {
      _device_info_index = 0;
      if (const auto m = parse_message(raw_device_count_response)) {
        return write_data(*m, buffer);
      }
    } else if (_last_sent_message->type == message_type::device_info &&
               _device_info_index &&
               *_device_info_index < raw_device_info_response.size()) {
      if (const auto m =
              parse_message(raw_device_info_response[*_device_info_index])) {
        _device_info_index =
            *_device_info_index == raw_device_info_response.size() - 1
                ? std::nullopt
                : std::optional{*_device_info_index + 1};
        return write_data(*m, buffer);
      }
    }

    return std::nullopt;
  }

private:
  std::optional<uint8_t> _device_info_index = 0;
  std::optional<message> _last_sent_message;
};
} // namespace

TEST_CASE("read_devices") {
  client client{mock_tcp_socket{}};
  std::array<uint8_t, 1024> buffer;
  std::vector<device> devices;
  REQUIRE(read_devices(client, buffer, devices));

  const auto expected_devices = std::vector<device>{
      unknown_device{},
      unknown_device{},
      unknown_device{},
      null_device{},
      null_device{},
      barometer_device{"Barometer", 3},
      pico_internal_device{5},
      null_device{},
      null_device{},
      null_device{},
      voltage_device{"ST107 [5596] 1", 11},
      voltage_device{"ST107 [5596] 2", 12},
      voltage_device{"ST107 [5596] 3", 13},
      resistive_device{"ST107 [5596] 1", 14},
      resistive_device{"ST107 [5596] 2", 15},
      resistive_device{"ST107 [5596] 3", 16},
      resistive_device{"ST107 [5596] 4", 17},
      unknown_device{},
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

} // namespace spymarine
