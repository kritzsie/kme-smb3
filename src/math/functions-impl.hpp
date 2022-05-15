#pragma once

#include "functions-decl.hpp"

#include "rect.hpp"
#include "vec2.hpp"

namespace kme::fp {
template<typename T>
constexpr Vec2<T> map(T (&f)(T), const Vec2<T>& vec2) {
  return Vec2<T>(f(vec2.x), f(vec2.y));
}

template<typename T>
constexpr Vec2<T> map(T (&f)(const T&), const Vec2<T>& vec2) {
  return Vec2<T>(f(vec2.x), f(vec2.y));
}

template<typename T>
constexpr Rect<T> map(T (&f)(T), const Rect<T>& rect) {
  return Rect<T>(map(f, rect.pos), map(f, rect.size));
}

template<typename T>
constexpr Rect<T> map(T (&f)(const T&), const Rect<T>& rect) {
  return Rect<T>(map(f, rect.pos), map(f, rect.size));
}
}
