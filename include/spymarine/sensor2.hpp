#pragma once

#include "spymarine/id.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/rational.hpp"
#include "spymarine/unit.hpp"

#include <optional>

namespace spymarine {

template <typename T, size_t Denominator, unit Unit> struct sensor_value2 {
  static constexpr auto unit = Unit;

  rational<T, Denominator> current_value{};
  float average_value{};

  sensor_value2() = default;

  constexpr sensor_value2(T raw_value) noexcept
      : current_value{rational<T, Denominator>{raw_value}},
        average_value{current_value.to_float()} {}

  constexpr void update(T raw_value, size_t average_count) noexcept {
    current_value = rational<T, Denominator>{raw_value};
    average_value =
        (average_value * float(average_count) + current_value.to_float()) /
        float(average_count + 1);
  }

  auto operator<=>(const sensor_value2&) const = default;
};

struct voltage_sensor2 {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value2<int16_t, 1000, unit::volts> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const voltage_sensor2&) const = default;
};

struct current_sensor2 {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value2<int16_t, 100, unit::amps> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const current_sensor2&) const = default;
};

struct temperature_sensor2 {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value2<int16_t, 10, unit::celsius> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const temperature_sensor2&) const = default;
};

struct barometer_sensor2 {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value2<int32_t, 100, unit::millibar> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.int32(), average_count);
  }

  auto operator<=>(const barometer_sensor2&) const = default;
};

struct resistive_sensor2 {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value2<int16_t, 1, unit::ohms> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const resistive_sensor2&) const = default;
};

using tank_volume = sensor_value2<int16_t, 10, unit::liters>;
using tank_level = sensor_value2<int16_t, 10, unit::percent>;

struct tank_sensor2 {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  tank_volume volume{};
  tank_level level{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    volume.update(numeric.high_int16(), average_count);
    level.update(numeric.low_int16(), average_count);
  }

  auto operator<=>(const tank_sensor2&) const = default;
};

using battery_charge = sensor_value2<int16_t, 160, unit::percent>;
using battery_remaining_capacity = sensor_value2<int16_t, 100, unit::amp_hours>;

struct battery_sensor2 {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  battery_charge charge{};
  battery_remaining_capacity remaining_capacity{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    charge.update(numeric.low_int16(), average_count);
    remaining_capacity.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const battery_sensor2&) const = default;
};

struct unsupported_sensor2 {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  uint8_t raw_type;
  uint32_t raw_value{};

  constexpr void update(const numeric_value1& numeric,
                        [[maybe_unused]] const size_t average_count) noexcept {
    raw_value = numeric.uint32();
  }

  auto operator<=>(const unsupported_sensor2&) const = default;
};

using sensor2 =
    std::variant<voltage_sensor2, current_sensor2, temperature_sensor2,
                 barometer_sensor2, resistive_sensor2, tank_sensor2,
                 battery_sensor2, unsupported_sensor2>;

constexpr void update_sensor(sensor2& sensor, const numeric_value1& value,
                             const size_t average_count) noexcept {
  std::visit([&](auto& s) { s.update(value, average_count); }, sensor);
}

} // namespace spymarine
