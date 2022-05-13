#pragma once

#include <type_traits>

#include <cmath>

namespace kme {
template<typename T,
  std::enable_if_t<not std::is_same_v<Sign, T>, std::nullptr_t>>
T operator *(const T& lhs, const Sign& rhs) {
  return rhs == Sign::PLUS ? lhs : -lhs;
}

template<typename T>
T operator *(const Sign& lhs, const T& rhs) {
  return lhs == Sign::PLUS ? rhs : -rhs;
}

template<typename T>
Sign toSign(const T& value) {
  return std::signbit(value) ? Sign::MINUS : Sign::PLUS;
}
}
