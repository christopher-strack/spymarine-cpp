#include "example_utils.hpp"

#include "spymarine/discover.hpp"
#include "spymarine/read_devices.hpp"
#include "spymarine/sensor_reader.hpp"

#include <print>

int main(int argc, char** argv) {
  std::println("Discover Simarine device");

  std::array<uint8_t, 2048> buffer;
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

            return spymarine::make_sensor_reader(buffer, devices)
                .and_then([](auto sensor_reader) {
                  std::println("Reading sensor states");
                  return sensor_reader.read_and_update();
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
