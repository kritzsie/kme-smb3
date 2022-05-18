#pragma once

#include "vec2.hpp"

#include <algorithm>

#include <cmath>

namespace kme::la {
template<typename T>
constexpr Vec2<T> normal(const Vec2<T>& vec2) {
  return vec2 / abs(vec2);
}

template<typename T, typename U>
constexpr U abs(const Vec2<T>& vec2) {
  return std::sqrt<U>(dot(vec2, vec2));
}

template<typename T, typename U>
constexpr U arg(const Vec2<T>& vec2) {
  return std::atan2<U>(vec2.y, vec2.x);
}

template<typename T, typename U>
constexpr U dot(const Vec2<T>& lhs, const Vec2<T>& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}
}
