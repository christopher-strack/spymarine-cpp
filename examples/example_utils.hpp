#pragma once

#include "spymarine/device_ostream.hpp"

#include <sstream>
#include <string>

inline std::string device_string(const spymarine::device& device) {
  std::stringstream str;
  str << device;
  return str.str();
}
