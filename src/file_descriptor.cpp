#include "spymarine/file_descriptor.hpp"

#include <unistd.h>
#include <utility>

file_descriptor::file_descriptor() noexcept : _fd{-1} {}
file_descriptor::file_descriptor(int fd) noexcept : _fd{fd} {}

file_descriptor::file_descriptor(file_descriptor&& other) noexcept
    : _fd{std::exchange(other._fd, -1)} {}

file_descriptor::~file_descriptor() {
  if (_fd != -1) {
    ::close(_fd);
  }
}

file_descriptor& file_descriptor::operator=(file_descriptor&& other) noexcept {
  _fd = std::exchange(other._fd, -1);
  return *this;
}

int file_descriptor::get() const noexcept { return _fd; }

bool file_descriptor::valid() const noexcept { return _fd > 0; }
