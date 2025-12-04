#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "data.hpp"
#include "spymarine/message_value.hpp"
#include "test_sockets.hpp"

#include "spymarine/client.hpp"
#include "spymarine/sensor2.hpp"

#include <catch2/catch_all.hpp>

#include <ranges>

namespace spymarine {

TEST_CASE("client") {
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

  SECTION("request_devices") {
    STATIC_CHECK(
        client{failing_tcp_socket{}, failing_udp_socket{}}.request_devices() ==
        std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_devices fails if socket fails") {
    STATIC_CHECK(
        client{failing_tcp_socket{}, failing_udp_socket{}}.request_devices() ==
        std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_device") {
    client cli{mock_tcp_socket{}, mock_udp_socket{}};
    const auto devices = std::views::iota(uint8_t(0), uint8_t(parsed_devices2.size())) |
                         std::views::transform([&](const auto id) {
                           return cli.request_device(id);
                         });

    CHECK_THAT(devices, Catch::Matchers::RangeEquals(parsed_devices2));
  }

  SECTION("request_sensor") {
    client cli{mock_tcp_socket{}, mock_udp_socket{}};
    const auto sensors =
        std::views::iota(sensor_id(0), sensor_id(parsed_sensors2.size())) |
        std::views::transform(
            [&](const sensor_id id) { return cli.request_sensor(id); });

    CHECK_THAT(sensors, Catch::Matchers::RangeEquals(parsed_sensors2));
  }

  SECTION("read_sensor_state") {
    client cli{mock_tcp_socket{}, mock_udp_socket{}};
    auto sensors =
        cli.request_count_info().transform([&](const auto count_info) {
          return std::views::iota(uint8_t(0), count_info.sensor_count) |
                 std::views::transform(
                     [&](auto id) { return cli.request_sensor(id); }) |
                 std::views::filter([](const auto& s) { return bool(s); }) |
                 std::views::transform([](const auto& s) { return *s; }) |
                 std::ranges::to<std::vector>();
        });

    if (auto values = cli.read_sensor_state()) {
      for (const auto& value : values.value()) {
        std::visit(overloaded{[&](const numeric_value1& n) {
                                update_sensor(sensors.value()[n.id()], n, 0);
                              },
                              [](const auto&) {
                                // ignore other value types
                              }},
                   value);
      }
    }

    CHECK(sensors.value() == std::vector<sensor2>{});
  }
}

} // namespace spymarine
