#include "example_utils.hpp"

#include "spymarine/buffer.hpp"
#include "spymarine/discover.hpp"
#include "spymarine/read_devices.hpp"
#include "spymarine/sensor_reader.hpp"
#include "spymarine/udp_socket.hpp"

#include <chrono>
#include <print>

namespace {

void process_sensor_values(
    const std::vector<spymarine::device>& devices,
    spymarine::moving_average_sensor_reader<spymarine::udp_socket>&
        sensor_reader) {
  std::println("Start processing sensor values");

  while (true) {
    const auto result =
        sensor_reader.read_and_update().transform([&](bool window_completed) {
          if (window_completed) {
            for (const auto& device : devices) {
              std::println("{}", device_string(device));
            }
          }
        });

    if (!result) {
      std::println("Failed to read sensor values: {}",
                   spymarine::error_message(result.error()));
    }
  }
}

} // namespace

int main(int argc, char** argv) {
  std::println("Discover Simarine device");

  spymarine::buffer buffer;
  const auto result =
      spymarine::discover()
          .and_then([&](const auto ip) {
            std::println("Read devices");

            spymarine::filter_by_device_type<spymarine::temperature_device,
                                             spymarine::tank_device,
                                             spymarine::battery_device>
                device_filter;

            return spymarine::read_devices<spymarine::tcp_socket>(
                buffer, ip, spymarine::simarine_default_tcp_port,
                device_filter);
          })
          .and_then([&](auto devices) {
            std::println("Found {} devices", devices.size());

            return spymarine::make_moving_average_sensor_reader(
                       buffer, std::chrono::seconds{10}, devices)
                .transform([&](auto sensor_reader) {
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
