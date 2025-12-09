#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "data.hpp"
#include "test_sockets.hpp"

#include "spymarine/hub.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("hub") {
  auto hub_ =
      initialize_hub_with_sockets(client{mock_tcp_socket{}, mock_udp_socket{}});
  REQUIRE(hub_.has_value());

  static const auto parsed_devices2 = make_parsed_devices2_with_sensors();
  CHECK_THAT(hub_->devices(), Catch::Matchers::RangeEquals(parsed_devices2));
  CHECK_THAT(hub_->sensors(), Catch::Matchers::RangeEquals(parsed_sensors2));

  CHECK(hub_->update_sensor_values().has_value());

  CHECK_THAT(hub_->sensors(),
             Catch::Matchers::RangeEquals(parsed_sensors2_with_value));
}

} // namespace spymarine
