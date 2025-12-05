#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "data.hpp"
#include "test_sockets.hpp"

#include "spymarine/buffer.hpp"
#include "spymarine/device_ostream.hpp" // IWYU pragma: keep
#include "spymarine/read_devices.hpp"

#include <catch2/catch_all.hpp>

#include <cstddef>
#include <expected>
#include <system_error>

namespace spymarine {

TEST_CASE("read_devices") {
  static_buffer buffer;

  SECTION("return parsed devices") {
    const auto devices =
        read_devices<mock_tcp_socket>(buffer, 0, 0, do_not_filter_devices{});

    CHECK(devices == make_parsed_devices());
  }

  SECTION("return connection error") {
    const auto devices =
        read_devices<failing_tcp_socket>(buffer, 0, 0, do_not_filter_devices{});

    REQUIRE_FALSE(devices);
    CHECK(std::holds_alternative<std::errc>(devices.error()));
  }
}

} // namespace spymarine
