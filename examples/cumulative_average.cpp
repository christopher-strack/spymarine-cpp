#include "example_utils.hpp"

#include "spymarine/discover.hpp"
#include "spymarine/read_devices.hpp"
#include "spymarine/sensor_reader.hpp"
#include "spymarine/udp_socket.hpp"

#include <print>

namespace {

void process_sensor_values(
    const std::vector<spymarine::device>& devices,
    spymarine::sensor_reader<spymarine::udp_socket>& sensor_reader) {
  std::println("Start processing sensor values");

  auto last_sent_print = std::chrono::steady_clock::now();
  while (true) {
    const auto delta = std::chrono::steady_clock::now() - last_sent_print;
    if (delta >= std::chrono::seconds{10}) {
      last_sent_print = std::chrono::steady_clock::now();

      for (const auto& device : devices) {
        std::println("{}", device_string(device));
      }

      [[maybe_unused]] const auto result =
          sensor_reader.read_and_update(spymarine::update_method::replace);
    } else {
      [[maybe_unused]] const auto result = sensor_reader.read_and_update(
          spymarine::update_method::cumulative_average);
    }
  }
}

} // namespace

int main(int argc, char** argv) {
  std::println("Discover Simarine device");

  const auto result =
      spymarine::discover()
          .transform_error(spymarine::error_from_error_code)
          .and_then([](const auto ip) {
            std::println("Read devices");

            spymarine::filter_by_device_type<spymarine::temperature_device,
                                             spymarine::tank_device,
                                             spymarine::battery_device>
                device_filter;

            return spymarine::read_devices<spymarine::tcp_socket>(
                ip, spymarine::simarine_default_tcp_port, device_filter);
          })
          .and_then([](auto devices) {
            std::println("Found {} devices", devices.size());

            return spymarine::make_sensor_reader(devices).transform(
                [&](auto sensor_reader) {
                  std::println("Reading sensor states");
                  process_sensor_values(devices, sensor_reader);
                });
          });

  if (!result) {
    std::println("Failed to read sensor states: {}",
                 spymarine::error_message(result.error()));
  }

  return 0;
}
