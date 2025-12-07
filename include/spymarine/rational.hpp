#pragma once

#include <cstddef>
#include <format>
#include <string>

namespace spymarine {

consteval size_t compute_precision(size_t denominator) {
  size_t d = denominator;
  size_t digits = 0;
  while (d > 1) {
    d /= 10;
    ++digits;
  }
  return digits;
}

template <typename T, size_t Denominator> class rational {
public:
  static constexpr size_t precision = compute_precision(Denominator);

  constexpr rational() noexcept = default;
  explicit constexpr rational(T numerator) noexcept : _numerator{numerator} {}

  constexpr T numerator() const noexcept { return _numerator; }

  constexpr float to_float() const noexcept {
    return static_cast<float>(_numerator) / static_cast<float>(Denominator);
  }

  constexpr std::string to_string() const {
    return std::format("{:.{}f}", to_float(), precision);
  }

  auto operator<=>(const rational<T, Denominator>&) const = default;

private:
  T _numerator;
};

} // namespace spymarine
