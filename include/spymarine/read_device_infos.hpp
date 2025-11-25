#pragma once

#include "spymarine/client.hpp"
#include "spymarine/device_info.hpp"
#include "spymarine/error.hpp"

#include <expected>
#include <ranges>
#include <vector>

namespace spymarine {

template <tcp_socket_concept tcp_socket_type>
constexpr std::expected<std::vector<device_info>, error>
read_device_infos(client<tcp_socket_type>& cl) noexcept {
  return cl.request_device_info_count().and_then(
      [&](const device_id count)
          -> std::expected<std::vector<device_info>, error> {
        std::vector<device_info> infos;
        infos.reserve(count);

        for (const auto id : std::views::iota(0u, count)) {
          std::expected<device_info, error> info = cl.request_device_info(id);

          if (!info) {
            return std::unexpected(info.error());
          }

          infos.emplace_back(std::move(*info));
        }

        return infos;
      });
}

} // namespace spymarine
