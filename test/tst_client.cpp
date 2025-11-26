#include "data.hpp"

#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "test_tcp_socket.hpp"

#include "spymarine/client.hpp"

#include <catch2/catch_all.hpp>

namespace spymarine {

TEST_CASE("client") {
  static constexpr auto infos = make_parsed_device_infos();

  SECTION("request_device_info_count returns valid count") {
    STATIC_CHECK(client{mock_tcp_socket{}}.request_device_info_count() ==
                 infos.size());
  }

  SECTION("request_device_info_count fails if socket fails") {
    STATIC_CHECK(client{failing_tcp_socket{}}.request_device_info_count() ==
                 std::unexpected(error{std::errc::connection_refused}));
  }

  SECTION("request_device_info") {
    const auto last_device_id = device_id(infos.size() - 1);
    STATIC_CHECK(client{mock_tcp_socket{}}.request_device_info(0) == infos[0]);
    STATIC_CHECK(client{mock_tcp_socket{}}.request_device_info(
                     last_device_id) == infos.back());
  }
}

} // namespace spymarine
