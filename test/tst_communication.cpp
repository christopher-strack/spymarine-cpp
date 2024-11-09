#include "raw_data.hpp"

#include "spymarine/communication.hpp"
#include "spymarine/device.hpp"
#include "spymarine/device_ostream.hpp"
#include "spymarine/parsing.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {
namespace {
struct mock_client {
  uint8_t device_info_response_index = 0;

  std::optional<message> request(const std::span<uint8_t>& data) {
    const auto m = parse_response(data);

    if (m->type == message_type::device_count) {
      device_info_response_index = 0;
      return parse_response(device_count_response);
    }
    if (m->type == message_type::device_info &&
        device_info_response_index < device_info_responses.size()) {
      return parse_response(
          device_info_responses[device_info_response_index++]);
    }
    return std::nullopt;
  }
};
} // namespace

TEST_CASE("read_devices") {
  mock_client client;
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
