#include "raw_data.hpp"

#include "spymarine/parse_message.hpp"

#include <catch2/catch_all.hpp>

#include <iterator>

namespace spymarine {

TEST_CASE("message_values_view") {
  BENCHMARK("small message") {
    constexpr auto msg = parse_message(device_count_message);
    static_assert(msg.has_value());

    return std::ranges::distance(msg->values());
  };

  BENCHMARK("large message") {
    constexpr auto msg = parse_message(device_info_message);
    static_assert(msg.has_value());

    return std::ranges::distance(msg->values());
  };
}

} // namespace spymarine
