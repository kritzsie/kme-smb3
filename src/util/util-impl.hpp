#pragma once

#include <cmath>

namespace kme {
template<typename T>
T operator *(const Sign& lhs, const T& rhs) {
  return lhs == Sign::PLUS ? rhs : -rhs;
}

template<typename T>
T operator *(const T& lhs, const Sign& rhs) {
  return rhs * lhs;
}

template<typename T>
Sign toSign(const T& value) {
  return std::copysign(1, value) >= 0 ? Sign::PLUS : Sign::MINUS;
}
}
