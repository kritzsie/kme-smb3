#pragma once

#include <algorithm>

#include <cmath>

namespace kme::util {
template<typename T>
constexpr T absdiv(const T& lhs, const T& rhs) {
  T q = lhs / rhs;
  T r = lhs % rhs;
  return (r != 0 && (r < 0) != (rhs < 0)) ? --q : q;
}

template<typename T>
constexpr T absmod(const T& lhs, const T& rhs) {
  return ((lhs % rhs) + rhs) % rhs;
}

template<>
constexpr float absmod<float>(const float& lhs, const float& rhs) {
  return std::fmod<float>(std::fmod<float>(lhs, rhs) + rhs, rhs);
}

template<>
constexpr double absmod<double>(const double& lhs, const double& rhs) {
  return std::fmod<double>(std::fmod<double>(lhs, rhs) + rhs, rhs);
}

template<>
constexpr long double absmod<long double>(const long double& lhs, const long double& rhs) {
  return std::fmod<long double>(std::fmod<long double>(lhs, rhs) + rhs, rhs);
}
}
