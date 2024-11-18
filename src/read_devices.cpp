#include "spymarine/read_devices.hpp"
#include "spymarine/overloaded.hpp"

namespace spymarine::detail {

namespace {
std::array<uint8_t, 2> to_bytes(uint16_t value) {
  return {uint8_t((value >> 8) & 0xff), uint8_t(value & 0xff)};
}

} // namespace

read_devices_error to_read_devices_error(const parse_error& err) { return err; }

std::span<uint8_t> write_message_data(message m, std::span<uint8_t> buffer) {
  const auto payload_size = header_size + m.data.size();
  const auto total_size = payload_size + 2;

  if (buffer.size() < total_size) {
    // buffer is too small for message
    std::abort();
  }

  const auto length = to_bytes(3 + m.data.size());
  const auto header = std::array<uint8_t, header_size>{
      0x00, 0x00, 0x00, 0x00, 0x00,      0xff,      uint8_t(m.type),
      0x04, 0x8c, 0x55, 0x4b, length[0], length[1], 0xff};

  auto it = std::copy(header.begin(), header.end(), buffer.begin());
  it = std::copy(m.data.begin(), m.data.end(), it);

  const auto calculated_crc =
      to_bytes(crc(std::span{buffer.begin() + 1, payload_size - 2}));
  std::copy(calculated_crc.begin(), calculated_crc.end(), it);

  return std::span{buffer.begin(), total_size};
}

} // namespace spymarine::detail

namespace spymarine {

std::string error_message(read_devices_error err) {
  return std::visit(overloaded{
                        [](parse_error e) { return std::to_string(int(e)); },
                        [](std::error_code ec) { return ec.message(); },
                    },
                    err);
}

} // namespace spymarine
