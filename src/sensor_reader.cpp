#include "spymarine/sensor_reader.hpp"
#include "spymarine/defaults.hpp"
#include "spymarine/overloaded.hpp"

namespace spymarine {

sensor_map build_sensor_map(std::vector<device>& devices_range) {
  sensor_map result;

  const auto insert_sensor = [&result](sensor& s) {
    auto it = result.find(s.state_index);
    if (it == result.end()) {
      it = result.emplace(s.state_index, std::vector<sensor*>{}).first;
    }
    it->second.push_back(std::addressof(s));
  };

  for (device& device : devices_range) {
    std::visit(overloaded{
                   [&](auto& d) { insert_sensor(d.device_sensor); },
                   [&](tank_device& d) {
                     insert_sensor(d.level_sensor);
                     insert_sensor(d.volume_sensor);
                   },
                   [&](battery_device& d) {
                     insert_sensor(d.charge_sensor);
                     insert_sensor(d.remaining_capacity_sensor);
                     insert_sensor(d.current_sensor);
                     insert_sensor(d.voltage_sensor);
                   },
               },
               device);
  }

  return result;
}

std::expected<sensor_reader<udp_socket>, error>
make_sensor_reader(std::vector<device>& devices) {
  auto bound_socket = udp_socket::open().and_then([&](auto socket) {
    return socket.bind(0, simarine_default_udp_port).transform([&]() {
      return std::move(socket);
    });
  });

  if (bound_socket) {
    return sensor_reader{devices, std::move(*bound_socket)};
  }

  return std::unexpected{bound_socket.error()};
}

} // namespace spymarine
