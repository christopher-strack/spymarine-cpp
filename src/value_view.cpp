#include "spymarine/value_view.hpp"

#include <algorithm>
#include <optional>

namespace spymarine {

numeric_value::numeric_value(std::span<const uint8_t, 4> bytes) {
  std::copy(bytes.begin(), bytes.end(), _bytes.begin());
}

uint16_t numeric_value::first() const { return (_bytes[0] << 8) | _bytes[1]; }

uint16_t numeric_value::second() const { return (_bytes[2] << 8) | _bytes[3]; }

uint32_t numeric_value::number() const {
  return (_bytes[0] << 24) | (_bytes[1] << 16) | (_bytes[2] << 8) | _bytes[3];
}

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

value_iterator::value_iterator() : _id_and_value{0, invalid_value{}} {}

value_iterator::value_iterator(std::span<const uint8_t> buffer)
    : _id_and_value{0, invalid_value{}}, _remaining{buffer} {
  update_id_and_value();
}

value_iterator::value_type value_iterator::operator*() const {
  return _id_and_value;
}

std::add_pointer_t<value_iterator::value_type const>
value_iterator::operator->() const {
  return std::addressof(_id_and_value);
}

value_iterator& value_iterator::operator++() {
  if (_remaining.size() < 2) {
    _remaining = {};
    return *this;
  }

  const auto value_type = _remaining[1];
  const auto bytes = _remaining.subspan(2);

  if (value_type == 1) {
    _remaining = advance_bytes(bytes, 5);
  } else if (value_type == 3) {
    _remaining = advance_bytes(bytes, 10);
  } else if (value_type == 4) {
    const auto string = std::get<std::string_view>(_id_and_value.value);
    _remaining = advance_bytes(bytes, string.size() + 7);
  } else {
    _remaining = {};
  }

  update_id_and_value();

  return *this;
}

value_iterator value_iterator::operator++(int) {
  value_iterator tmp = *this;
  ++(*this);
  return tmp;
}

void value_iterator::update_id_and_value() {
  if (_remaining.size() < 2) {
    return;
  }

  _id_and_value.id = _remaining[0];

  const auto value_type = _remaining[1];
  const auto bytes = _remaining.subspan(2);

  if (value_type == 1 && bytes.size() >= 4) {
    _id_and_value.value = numeric_value{bytes.subspan<0, 4>()};
  } else if (value_type == 3 && bytes.size() >= 9) {
    _id_and_value.value = numeric_value{bytes.subspan<5, 4>()};
  } else if (value_type == 4) {
    const auto string = read_string(bytes);
    _id_and_value.value = string ? value{*string} : invalid_value{};
  } else {
    _id_and_value.value = invalid_value{};
  }
}

bool operator==(const value_iterator& lhs, const value_iterator& rhs) {
  return std::ranges::equal(lhs._remaining, rhs._remaining);
}

bool operator!=(const value_iterator& lhs, const value_iterator& rhs) {
  return !(lhs == rhs);
}

value_view::value_view(std::span<const uint8_t> buffer) : _buffer{buffer} {}

value_iterator value_view::begin() const { return value_iterator(_buffer); }
value_iterator value_view::end() const {
  return value_iterator(std::span<const uint8_t>{});
}

value_iterator find_value(const uint8_t id, const value_view& values) {
  return std::ranges::find_if(
      values, [id](const auto value) { return value.id == id; });
}

} // namespace spymarine
