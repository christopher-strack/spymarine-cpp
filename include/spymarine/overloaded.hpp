#pragma once

namespace spymarine {

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

} // namespace spymarine
