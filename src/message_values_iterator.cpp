#include "spymarine/message_values_iterator.hpp"

#include <algorithm>
#include <optional>

namespace spymarine {
namespace {

std::optional<std::string_view> read_string(std::span<const uint8_t> bytes) {
  if (bytes.size() >= 5) {
    const auto data = bytes.subspan(5);
    const auto it = std::ranges::find(data, 0);
    if (it != data.end()) {
      const auto size = static_cast<size_t>(std::distance(data.begin(), it));
      return std::string_view{reinterpret_cast<const char*>(data.data()), size};
    }
  }
  return std::nullopt;
}

std::span<const uint8_t> advance_bytes(std::span<const uint8_t> bytes,
                                       size_t n) {
  return bytes.size() >= n + 2 ? bytes.subspan(n) : std::span<const uint8_t>{};
}

} // namespace

message_values_iterator::message_values_iterator()
    : _data{0, invalid_value{}} {}

message_values_iterator::message_values_iterator(
    std::span<const uint8_t> buffer)
    : _data{0, invalid_value{}}, _bytes{buffer} {
  update_data();
}

message_values_iterator::value_type message_values_iterator::operator*() const {
  return _data;
}

std::add_pointer_t<message_values_iterator::value_type const>
message_values_iterator::operator->() const {
  return std::addressof(_data);
}

message_values_iterator& message_values_iterator::operator++() {
  if (_bytes.size() < 2) {
    _bytes = {};
    return *this;
  }

  const auto value_type = _bytes[1];

  if (value_type == 1) {
    _bytes = advance_bytes(_bytes, 7);
  } else if (value_type == 3) {
    _bytes = advance_bytes(_bytes, 12);
  } else if (value_type == 4) {
    const auto string = std::get<std::string_view>(_data.value);
    _bytes = advance_bytes(_bytes, string.size() + 9);
  } else {
    _bytes = {};
  }

  update_data();

  return *this;
}

message_values_iterator message_values_iterator::operator++(int) {
  message_values_iterator tmp = *this;
  ++(*this);
  return tmp;
}

void message_values_iterator::update_data() {
  if (_bytes.size() < 2) {
    return;
  }

  _data.id = _bytes[0];

  const auto value_type = _bytes[1];
  const auto payload = _bytes.subspan(2);

  if (value_type == 1 && payload.size() >= 4) {
    _data.value = numeric_value{payload.subspan<0, 4>()};
  } else if (value_type == 3 && payload.size() >= 9) {
    _data.value = numeric_value{payload.subspan<5, 4>()};
  } else if (value_type == 4) {
    const auto string = read_string(payload);
    _data.value = string ? message_value{*string} : invalid_value{};
  } else {
    _data.value = invalid_value{};
  }
}

bool operator==(const message_values_iterator& lhs,
                const message_values_iterator& rhs) {
  return std::ranges::equal(lhs._bytes, rhs._bytes);
}

bool operator!=(const message_values_iterator& lhs,
                const message_values_iterator& rhs) {
  return !(lhs == rhs);
}

} // namespace spymarine
