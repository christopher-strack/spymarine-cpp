#pragma once

#include "spymarine/id.hpp"
#include "spymarine/message_value.hpp"
#include "spymarine/message_values_view.hpp"
#include "spymarine/rational.hpp"
#include "spymarine/unit.hpp"

#include <cassert>
#include <concepts> // IWYU pragma: keep
#include <optional>

namespace spymarine {

template <typename T, size_t Denominator, unit Unit> struct sensor_value {
  static constexpr auto unit = Unit;

  rational<T, Denominator> current_value{};
  float average_value{};

  sensor_value() = default;

  constexpr sensor_value(T raw_value) noexcept
      : current_value{rational<T, Denominator>{raw_value}},
        average_value{current_value.to_float()} {}

  constexpr void update(T raw_value, size_t average_count) noexcept {
    current_value = rational<T, Denominator>{raw_value};
    average_value =
        (average_value * float(average_count) + current_value.to_float()) /
        float(average_count + 1);
  }

  auto operator<=>(const sensor_value&) const = default;
};

struct voltage_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value<int16_t, 1000, unit::volts> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const voltage_sensor&) const = default;
};

struct current_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value<int16_t, 100, unit::amps> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const current_sensor&) const = default;
};

struct temperature_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value<int16_t, 10, unit::celsius> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const temperature_sensor&) const = default;
};

struct barometer_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value<int32_t, 100, unit::millibar> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.int32(), average_count);
  }

  auto operator<=>(const barometer_sensor&) const = default;
};

struct barometer_trend_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value<int32_t, 100, unit::millibars_per_hour> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.int32(), average_count);
  }

  auto operator<=>(const barometer_trend_sensor&) const = default;
};

struct resistive_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  sensor_value<int16_t, 1, unit::ohms> value{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    value.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const resistive_sensor&) const = default;
};

using tank_volume = sensor_value<int16_t, 10, unit::liters>;
using tank_level = sensor_value<int16_t, 10, unit::percent>;

struct tank_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  tank_volume volume{};
  tank_level level{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    volume.update(numeric.high_int16(), average_count);
    level.update(numeric.low_int16(), average_count);
  }

  auto operator<=>(const tank_sensor&) const = default;
};

using battery_charge = sensor_value<int16_t, 160, unit::percent>;
using battery_remaining_capacity = sensor_value<int16_t, 100, unit::amp_hours>;

struct battery_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  battery_charge charge{};
  battery_remaining_capacity remaining_capacity{};

  constexpr void update(const numeric_value1& numeric,
                        const size_t average_count) noexcept {
    charge.update(numeric.low_int16(), average_count);
    remaining_capacity.update(numeric.high_int16(), average_count);
  }

  auto operator<=>(const battery_sensor&) const = default;
};

struct unsupported_sensor {
  sensor_id id;
  std::optional<device_id> parent_device_id;
  uint8_t raw_type;
  uint32_t raw_value{};

  constexpr void update(const numeric_value1& numeric,
                        [[maybe_unused]] const size_t average_count) noexcept {
    raw_value = numeric.uint32();
  }

  auto operator<=>(const unsupported_sensor&) const = default;
};

using sensor =
    std::variant<voltage_sensor, current_sensor, temperature_sensor,
                 barometer_sensor, barometer_trend_sensor, resistive_sensor,
                 tank_sensor, battery_sensor, unsupported_sensor>;

template <typename T>
concept sensor_range =
    std::ranges::random_access_range<T> && std::ranges::sized_range<T> &&
    std::same_as<std::ranges::range_value_t<T>, sensor>;

constexpr sensor_id get_sensor_id(const sensor& sen) noexcept {
  return std::visit([](const auto& s) { return s.id; }, sen);
}

constexpr auto get_parent_device_id(const sensor& sen) noexcept {
  return std::visit([&](auto& s) { return s.parent_device_id; }, sen);
}

constexpr void update_sensor(sensor& sen, const numeric_value1& value,
                             const size_t average_count) noexcept {
  std::visit([&](auto& s) { s.update(value, average_count); }, sen);
}

constexpr void update_sensor_values(sensor_range auto& sensors,
                                    const message_values_view& values,
                                    const size_t average_count) {
  for (const auto& value : values) {
    if (const auto num = std::get_if<numeric_value1>(&value);
        num != nullptr && num->id() < sensors.size()) {
      auto& sen = sensors[num->id()];
      assert(get_sensor_id(sen) == num->id());
      update_sensor(sen, *num, average_count);
    }
  }
}

} // namespace spymarine
