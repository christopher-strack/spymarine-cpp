#include "catch_string_maker.hpp" // IWYU pragma: keep
#include "data.hpp"
#include "test_tcp_socket.hpp"

#include "spymarine/read_device_infos.hpp"

#include <catch2/catch_all.hpp>

#include <ranges>

namespace spymarine {

TEST_CASE("read_device_infos") {
  client cl{mock_tcp_socket{}};
  const auto expected_infos =
      make_parsed_device_infos() | std::ranges::to<std::vector<device_info>>();
  REQUIRE(read_device_infos(cl) == expected_infos);
}

} // namespace spymarine
