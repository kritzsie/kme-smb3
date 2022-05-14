#pragma once

#include "vec2.hpp"

namespace kme::la {
template<typename T>
constexpr Vec2<T> normal(const Vec2<T>& vec2);

template<typename T, typename U = float>
constexpr U abs(const Vec2<T>& vec2);

template<typename T, typename U = float>
constexpr U arg(const Vec2<T>& vec2);

template<typename T, typename U = float>
constexpr U dot(const Vec2<T>& lhs, const Vec2<T>& rhs);
}
