#pragma once

#include "rect.hpp"
#include "vec2.hpp"

#include <cstddef>

namespace kme::la {
template<typename T, typename U>
constexpr U map(const U& self, T (&f)(T));

template<typename T>
constexpr Vec2<T> normalized(const Vec2<T>& self);

template<typename T, typename U = double>
constexpr U abs(const Vec2<T>& self);

template<typename T, typename U = double>
constexpr U arg(const Vec2<T>& self);

template<typename T, typename U = double>
constexpr U dot(const Vec2<T>& lhs, const Vec2<T>& rhs);

template<typename T>
constexpr bool contains(const Rect<T>& self, const Vec2<T>& point);

template<typename T>
constexpr bool contains(const Rect<T>& self, const Rect<T>& set);

template<typename T>
constexpr bool intersects(const Rect<T>& lhs, const Rect<T>& rhs);

template<typename T>
constexpr Rect<T> intersection(const Rect<T>& lhs, const Rect<T>& rhs);

template<typename T>
constexpr Vec2<T> midpoint(const Rect<T>& self);

template<typename T>
constexpr Vec2<T> radius(const Rect<T>& self);
}
