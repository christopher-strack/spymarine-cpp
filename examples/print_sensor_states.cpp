#include "example_utils.hpp"

#include "spymarine/discover.hpp"
#include "spymarine/read_devices.hpp"
#include "spymarine/sensor_reader.hpp"

#include <print>

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

            return spymarine::make_sensor_reader(devices)
                .and_then([](auto sensor_reader) {
                  std::println("Reading sensor states");
                  return sensor_reader.update();
                })
                .transform([&]() {
                  for (const auto& device : devices) {
                    std::println("{}", device_string(device));
                  }
                });
          });

  if (!result) {
    std::println("Failed to read sensor states: {}",
                 spymarine::error_message(result.error()));
  }

  return 0;
}
