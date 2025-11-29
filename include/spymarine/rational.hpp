#pragma once

#include <cstddef>

namespace spymarine {

enum class unit {
  volts,
  amps,
  celsius,
  millibar,
  ohms,
  liters,
  percent,
  amp_hours,
};

template <typename T, size_t Denominator, unit Unit> class rational {
public:
  static constexpr auto unit = Unit;

  explicit constexpr rational(T numerator) noexcept : _numerator{numerator} {}

  constexpr T numerator() const noexcept { return _numerator; }

  constexpr float to_float() const noexcept {
    return static_cast<float>(_numerator) / static_cast<float>(Denominator);
  }

  auto operator<=>(const rational<T, Denominator, Unit>&) const = default;

private:
  T _numerator;
};

} // namespace spymarine
