#pragma once

#include <ctgmath>

namespace kme::util {
template<typename T>
constexpr T absdiv(T lhs, T rhs) {
  T q = lhs / rhs;
  T r = lhs % rhs;
  return (r != 0 && (r < 0) != (rhs < 0)) ? --q : q;
}

template<typename T>
constexpr T absmod(T lhs, T rhs) {
  return ((lhs % rhs) + rhs) % rhs;
}

template<>
constexpr float absmod<float>(float lhs, float rhs) {
  return std::fmod<float>(std::fmod<float>(lhs, rhs) + rhs, rhs);
}

template<>
constexpr double absmod<double>(double lhs, double rhs) {
  return std::fmod<double>(std::fmod<double>(lhs, rhs) + rhs, rhs);
}

template<>
constexpr long double absmod<long double>(long double lhs, long double rhs) {
  return std::fmod<long double>(std::fmod<long double>(lhs, rhs) + rhs, rhs);
}
}
