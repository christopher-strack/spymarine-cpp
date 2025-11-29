#pragma once

#include "spymarine/device_info.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/rational.hpp"

#include <functional>
#include <variant>

namespace spymarine {

struct voltage_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 1000, unit::volts>(_raw_value.high_int16());
  }
};

struct current_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 100, unit::amps>(_raw_value.high_int16());
  }
};

struct temperature_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 10, unit::celsius>(_raw_value.high_int16());
  }
};

struct barometer_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 100, unit::millibar>(_raw_value.high_int16());
  }
};

struct resistive_type {
  static constexpr auto convert(const numeric_value1& _raw_value) noexcept {
    return rational<int16_t, 1, unit::ohms>(_raw_value.high_int16());
  }
};

using tank_volume = rational<int16_t, 10, unit::liters>;
using tank_level = rational<int16_t, 10, unit::percent>;

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

using battery_charge = rational<int16_t, 160, unit::percent>;
using battery_capacity = rational<int16_t, 100, unit::amp_hours>;

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

using sensor_id = uint8_t;

template <typename sensor_type> class sensor2 {
public:
  using type = sensor_type;

  constexpr explicit sensor2(const sensor_id id) noexcept
      : _id{id}, _raw_bytes{} {}

  sensor_id id() const noexcept { return _id; }

  constexpr auto value() const noexcept {
    return type::convert(numeric_value1{_raw_bytes});
  }

  void set_raw_value(const numeric_value1& value) noexcept {
    assert(value.id() == _id);
    std::ranges::copy(value.raw_bytes(), _raw_bytes.begin());
  }

  constexpr auto operator==(const sensor2& other) const noexcept {
    return numeric_value1{_raw_bytes} == numeric_value1{other._raw_bytes};
  }

  constexpr auto operator<=>(const sensor2& other) const noexcept {
    return numeric_value1{_raw_bytes} <=> numeric_value1{other._raw_bytes};
  }

private:
  sensor_id _id;
  std::array<uint8_t, numeric_value1::raw_bytes_size()> _raw_bytes;
};

using any_sensor_ref =
    std::variant<std::reference_wrapper<sensor2<voltage_type>>,
                 std::reference_wrapper<sensor2<current_type>>,
                 std::reference_wrapper<sensor2<temperature_type>>,
                 std::reference_wrapper<sensor2<barometer_type>>,
                 std::reference_wrapper<sensor2<resistive_type>>,
                 std::reference_wrapper<sensor2<tank_type>>,
                 std::reference_wrapper<sensor2<battery_charge_type>>>;

constexpr void set_sensor_value(any_sensor_ref sensor_ref,
                                const numeric_value1& value) noexcept {
  std::visit([&](auto sr) { sr.get().set_raw_value(value); }, sensor_ref);
}

} // namespace spymarine
