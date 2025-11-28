#pragma once

#include "spymarine/device_info.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/rational.hpp"

namespace spymarine {

struct voltage_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 1000>(_raw_value.high_int16());
  }
};

struct current_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 100>(_raw_value.high_int16());
  }
};

struct temperature_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 10>(_raw_value.high_int16());
  }
};

struct barometer_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 100>(_raw_value.high_int16());
  }
};

struct resistive_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return _raw_value.high_int16();
  }
};

using tank_volume = rational<int16_t, 10>;
using tank_level = rational<int16_t, 10>;

struct tank_values {
  tank_volume volume;
  tank_level level;
};

struct tank_type {
  static constexpr tank_values
  convert(const numeric_value1& _raw_value) noexcept {
    return tank_values{
        tank_volume{_raw_value.high_int16()},
        tank_level{_raw_value.low_int16()},
    };
  }
};

using battery_charge = rational<int16_t, 160>;
using battery_capacity = rational<int16_t, 100>;

struct battery_charge_values {
  battery_charge charge;
  battery_capacity remaining_capacity;
};

struct battery_charge_type {
  static constexpr battery_charge_values
  convert(const numeric_value1& _raw_value) noexcept {
    return battery_charge_values{
        battery_charge{_raw_value.low_int16()},
        battery_capacity{_raw_value.high_int16()},
    };
  }
};

using sensor_id = int32_t;

template <typename sensor_type> class sensor2 {
public:
  using type = sensor_type;

  constexpr explicit sensor2(const sensor_id id) noexcept
      : _id{id}, _raw_bytes{}, _raw_value{_raw_bytes} {}

  constexpr auto value() noexcept { return type::convert(_raw_value); }

  void set_raw_value(const numeric_value1& value) noexcept {
    std::ranges::copy(value.raw_bytes(), _raw_bytes.begin());
  }

  constexpr auto operator==(const sensor2& other) const noexcept {
    return _raw_value == other._raw_value;
  }

  constexpr auto operator<=>(const sensor2& other) const noexcept {
    return _raw_value <=> other._raw_value;
  }

private:
  sensor_id _id;
  std::array<uint8_t, numeric_value1::raw_bytes_size()> _raw_bytes;
  numeric_value1 _raw_value;
};

} // namespace spymarine
