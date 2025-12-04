#pragma once

#include <cstddef>

namespace spymarine {

template <typename T, size_t Denominator> class rational {
public:
  constexpr rational() noexcept = default;
  explicit constexpr rational(T numerator) noexcept : _numerator{numerator} {}

  constexpr T numerator() const noexcept { return _numerator; }

  constexpr float to_float() const noexcept {
    return static_cast<float>(_numerator) / static_cast<float>(Denominator);
  }

  auto operator<=>(const rational<T, Denominator>&) const = default;

private:
  T _numerator;
};

} // namespace spymarine
