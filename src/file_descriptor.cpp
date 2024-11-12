#include "spymarine/file_descriptor.hpp"

#include <unistd.h>
#include <utility>

file_descriptor::file_descriptor() noexcept : fd{-1} {}
file_descriptor::file_descriptor(int fd) noexcept : fd{fd} {}

file_descriptor::file_descriptor(file_descriptor&& other) noexcept
    : fd{std::exchange(other.fd, -1)} {}

file_descriptor::~file_descriptor() {
  if (fd != -1) {
    ::close(fd);
  }
}

file_descriptor& file_descriptor::operator=(file_descriptor&& other) noexcept {
  fd = std::exchange(other.fd, -1);
  return *this;
}

int file_descriptor::get() const noexcept { return fd; }

bool file_descriptor::valid() const noexcept { return fd > 0; }
