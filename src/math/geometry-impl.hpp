#pragma once

#include "geometry-decl.hpp"

#include "rect.hpp"
#include "vec2.hpp"

namespace kme::geo {
template<typename T>
constexpr bool contains(const Rect<T>& set, const Vec2<T>& point) {
  return point.x >= set.x and point.x <= set.x + set.width
  and    point.y >= set.y and point.y <= set.y + set.height;
}

template<typename T>
constexpr bool contains(const Rect<T>& set, const Rect<T>& subset) {
  return subset.x >= set.x
  and    subset.x + subset.width <= set.x + set.width
  and    subset.y >= set.y
  and    subset.y + subset.height <= set.y + set.height;
}

template<typename T>
constexpr bool intersects(const Rect<T>& lhs, const Rect<T>& rhs) {
  return lhs.x < rhs.x + rhs.width
  and    lhs.x + lhs.width > rhs.x
  and    lhs.y < rhs.y + rhs.height
  and    lhs.y + lhs.height > rhs.y;
}

template<typename T>
constexpr Vec2<T> midpoint(const Rect<T>& rect) {
  return rect.pos + radius(rect);
}

template<typename T>
constexpr Vec2<T> radius(const Rect<T>& rect) {
  return rect.size / 2;
}
}
