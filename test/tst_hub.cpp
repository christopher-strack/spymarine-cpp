#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "data.hpp"
#include "spymarine/system_info.hpp"
#include "test_sockets.hpp"

#include "spymarine/hub.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("initialize_hub") {
  auto h =
      initialize_basic_hub(basic_client{mock_tcp_socket{}, mock_udp_socket{}});
  REQUIRE(h.has_value());

  const auto parsed_devices = make_parsed_devices_with_sensors();
  CHECK(h->system() == parsed_system_info);
  CHECK_THAT(h->all_devices(), Catch::Matchers::RangeEquals(parsed_devices));
  CHECK_THAT(h->all_sensors(),
             Catch::Matchers::RangeEquals(parsed_sensors_with_value));
}

} // namespace spymarine
