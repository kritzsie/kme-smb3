#pragma once

#include "vec2.hpp"

#include <algorithm>

#include <cmath>

namespace kme::la {
template<typename T>
constexpr Vec2<T> map(const Vec2<T>& self, T (&f)(T)) {
  return Vec2<T>(f(self.x), f(self.y));
}

template<typename T>
constexpr Vec2<T> normalized(const Vec2<T>& self) {
  return self / length(self);
}

template<typename T, typename U>
constexpr U length(const Vec2<T>& self) {
  return std::sqrt<U>(self.x * self.x + self.y * self.y);
}

template<typename T, typename U>
constexpr U arg(const Vec2<T>& self) {
  return std::atan2<U>(self.y, self.x);
}

template<typename T, typename U>
constexpr U dot(const Vec2<T>& lhs, const Vec2<T>& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename T>
constexpr Rect<T> map(const Rect<T>& self, T (&f)(T)) {
  return Rect<T>(map(self.pos, f), map(self.size, f));
}

template<typename T>
constexpr bool contains(const Rect<T>& region, const Vec2<T>& point) {
  return point.x >= region.pos.x and point.x <= region.pos.x + region.size.x
  and    point.y >= region.pos.y and point.y <= region.pos.y + region.size.y;
}

template<typename T>
constexpr bool contains(const Rect<T>& region, const Rect<T>& subset) {
  return subset.pos.x >= region.pos.x
  and    subset.pos.x + subset.size.x <= region.pos.x + region.size.x
  and    subset.pos.y >= region.pos.y
  and    subset.pos.y + subset.size.y <= region.pos.y + region.size.y;
}

template<typename T>
constexpr bool intersects(const Rect<T>& lhs, const Rect<T>& rhs) {
  return lhs.pos.x < rhs.pos.x + rhs.size.x
  and    lhs.pos.x + lhs.size.x > rhs.pos.x
  and    lhs.pos.y < rhs.pos.y + rhs.size.y
  and    lhs.pos.y + lhs.size.y > rhs.pos.y;
}

template<typename T>
constexpr Rect<T> intersection(const Rect<T>& lhs, const Rect<T>& rhs) {
  return Rect<T>(
    std::max<T>(lhs.pos.x, rhs.pos.x),
    std::max<T>(lhs.pos.y, rhs.pos.y),
    lhs.pos.x < rhs.pos.x \
    ? lhs.pos.x + lhs.size.x - rhs.pos.x
    : rhs.pos.x + rhs.size.x - lhs.pos.x,
    lhs.pos.y < rhs.pos.y \
    ? lhs.pos.y + lhs.size.y - rhs.pos.y
    : rhs.pos.y + rhs.size.y - lhs.pos.y
  );
}

template<typename T>
constexpr Vec2<T> midpoint(const Rect<T>& self) {
  return self.pos + radius(self);
}

template<typename T>
constexpr Vec2<T> radius(const Rect<T>& self) {
  return self.size / 2;
}
}
