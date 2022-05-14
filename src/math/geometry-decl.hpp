#pragma once

#include "rect.hpp"
#include "vec2.hpp"

namespace kme::geo {
template<typename T>
constexpr bool contains(const Rect<T>& set, const Vec2<T>& point);

template<typename T>
constexpr bool contains(const Rect<T>& set, const Rect<T>& subset);

template<typename T>
constexpr bool intersects(const Rect<T>& lhs, const Rect<T>& rhs);

template<typename T>
constexpr Vec2<T> midpoint(const Rect<T>& rect);

template<typename T>
constexpr Vec2<T> radius(const Rect<T>& rect);
}
