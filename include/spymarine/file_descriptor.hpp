#pragma once

class file_descriptor {
public:
  file_descriptor() noexcept;
  explicit file_descriptor(int fd) noexcept;
  ~file_descriptor();

  file_descriptor(file_descriptor&& fd) noexcept;
  file_descriptor& operator=(file_descriptor&& other) noexcept;

  file_descriptor(const file_descriptor& fd) = delete;
  file_descriptor& operator=(const file_descriptor& other) = delete;

  int get() const noexcept;
  bool valid() const noexcept;

private:
  int _fd;
};
