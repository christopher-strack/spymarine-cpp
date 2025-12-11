#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "data.hpp"
#include "test_sockets.hpp"

#include "spymarine/client.hpp"
#include "spymarine/sensor2.hpp"

#include <catch2/catch_all.hpp>

#include <ranges>

namespace spymarine {

TEST_CASE("client") {
  static const auto parsed_devices2 = make_parsed_devices2();

  SECTION("request_count_info") {
    STATIC_CHECK(
        client{mock_tcp_socket{}, mock_udp_socket{}}.request_count_info() ==
        count_info{.device_count = 32, .sensor_count = 42});
  }

  SECTION("request_count_info fails if socket fails") {
    STATIC_CHECK(client{failing_tcp_socket{}, failing_udp_socket{}}
                     .request_count_info() ==
                 std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_device") {
    CHECK(client{mock_tcp_socket{}, mock_udp_socket{}}.request_device(0) ==
          parsed_devices2[0]);
  }

  SECTION("request_device fails if socket fails") {
    STATIC_CHECK(
        client{failing_tcp_socket{}, failing_udp_socket{}}.request_device(0) ==
        std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_device integration") {
    client cli{mock_tcp_socket{}, mock_udp_socket{}};
    const auto devices =
        std::views::iota(uint8_t(0), uint8_t(parsed_devices2.size())) |
        std::views::transform(
            [&](const auto id) { return cli.request_device(id); });

    CHECK_THAT(devices, Catch::Matchers::RangeEquals(parsed_devices2));
  }

  SECTION("request_sensor") {
    STATIC_CHECK(client{mock_tcp_socket{}, mock_udp_socket{}}.request_sensor(
                     0) == parsed_sensors2[0]);
  }

  SECTION("request_sensor fails if socket fails") {
    STATIC_CHECK(
        client{failing_tcp_socket{}, failing_udp_socket{}}.request_sensor(0) ==
        std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_sensor integration") {
    client cli{mock_tcp_socket{}, mock_udp_socket{}};
    const auto sensors =
        std::views::iota(sensor_id(0), sensor_id(parsed_sensors2.size())) |
        std::views::transform(
            [&](const sensor_id id) { return cli.request_sensor(id); });

    CHECK_THAT(sensors, Catch::Matchers::RangeEquals(parsed_sensors2));
  }

  SECTION("discover") {
    auto ip = discover_with_socket<mock_udp_socket>(0);
    REQUIRE(ip.has_value());
  }

  SECTION("connect") {
    auto client_ = connect_with_sockets<mock_tcp_socket, mock_udp_socket>(0);
    REQUIRE(client_.has_value());
  }
}

} // namespace spymarine
