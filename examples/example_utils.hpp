#pragma once

#include "spymarine/device_ostream.hpp"

#include <sstream>
#include <string>

inline std::string device_string(const spymarine::device& d) {
  std::stringstream str;
  str << d;
  return str.str();
}
