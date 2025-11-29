#include "data.hpp"

#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "spymarine/device_list.hpp"
#include "test_sockets.hpp"

#include "spymarine/client.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("client") {
  static constexpr auto infos = make_parsed_device_infos();

  SECTION("request_device_infos") {
    STATIC_CHECK(client{failing_tcp_socket{}, failing_udp_socket{}}
                     .request_device_infos() ==
                 std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_device_infos fails if socket fails") {
    STATIC_CHECK(client{failing_tcp_socket{}, failing_udp_socket{}}
                     .request_device_infos() ==
                 std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_device_info_count returns valid count") {
    STATIC_CHECK(client{mock_tcp_socket{}, mock_udp_socket{}}
                     .request_device_info_count() == infos.size());
  }

  SECTION("request_device_info_count fails if socket fails") {
    STATIC_CHECK(client{failing_tcp_socket{}, failing_udp_socket{}}
                     .request_device_info_count() ==
                 std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_device_info") {
    const auto last_device_id = device_id(infos.size() - 1);
    STATIC_CHECK(
        client{mock_tcp_socket{}, mock_udp_socket{}}.request_device_info(0) ==
        infos[0]);
    STATIC_CHECK(
        client{mock_tcp_socket{}, mock_udp_socket{}}.request_device_info(
            last_device_id) == infos.back());
  }

  SECTION("read_sensor_values") {
    client cli{mock_tcp_socket{}, mock_udp_socket{}};
    if (auto device_infos = cli.request_device_infos()) {
      device_list devs{*device_infos};

      if (auto numbers = cli.read_sensor_values()) {
        devs.update_sensor_values(*numbers);

        CHECK(devs.get() == std::vector<device2>{});
      }
    }
  }
}

} // namespace spymarine
