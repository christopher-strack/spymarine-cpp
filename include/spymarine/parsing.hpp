#include <iosfwd>
#include <optional>
#include <span>
#include <string>

namespace spymarine {

constexpr auto header_length = 14;

/* Describes the header of a message
 */
struct header {
  uint8_t type;
  uint16_t length;
};

bool operator==(const header& lhs, const header& rhs);
bool operator!=(const header& lhs, const header& rhs);

/* The message type described by the header.
 * enum only represents the known subset.
 */
enum class message_type {
  // Request the number of connected devices
  device_count = 0x02,

  // Request information about a device
  device_info = 0x41,

  // Sensor update message (UDP)
  sensor_state = 0xb0,
};

/* A message as received by a Simarine device
 */
struct message {
  message_type type;
  std::span<const uint8_t> data;
};

bool operator==(const message& lhs, const message& rhs);
bool operator!=(const message& lhs, const message& rhs);

/* Parses the given bytes. Returns a Header on success.
    Raises ParsingError if the given bytes are not a valid header.
    Note: The header is not fully understood and might not work on
    all Simarine devices.
 */
std::optional<header> parse_header(const std::span<const uint8_t> data);

/* Calculate a CRC as accepted by Simarine devices.

  Original source: https://github.com/htool/pico2signalk
  Copyright Erik Bosman / @brainsmoke
*/
uint16_t crc(const std::span<const uint8_t> bytes);

/* Parse a response from a Simarine device. Returns a Message on success.
 * Raises ParsingError if the given data is not a valid or known Simarine
 * Message.
 */
std::optional<message>
parse_response(const std::span<const uint8_t> raw_response);

struct property_dict {
  std::unordered_map<uint8_t, std::string> strings;
  std::unordered_map<uint8_t, int32_t> numbers;
};

/* Converts bytes received by a Simarine devices to a ValueMap.
   Raises ParsingError in case the given bytes do not contain a valid
   ValueMap.
*/
property_dict parse_property_dict(std::span<const uint8_t> bytes);

enum class device_type {
  null,
  pico_internal,
  voltage,
  current,
  temperature,
  barometer,
  resistive,
  tank,
  battery,
  unknown,
};

struct device {
  device_type type;
  std::string name;
};

bool operator==(const device& lhs, const device& rhs);
bool operator!=(const device& lhs, const device& rhs);
std::ostream& operator<<(std::ostream& stream, const device& device);

device device_from_property_dict(const property_dict& map);

} // namespace spymarine
