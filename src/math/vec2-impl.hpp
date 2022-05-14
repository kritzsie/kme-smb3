#pragma once

#include "vec2-decl.hpp"

#include <functional>
#include <type_traits>

namespace kme {
template<typename T>
constexpr Vec2<T>::Vec2() : Vec2(T(), T()) {}

template<typename T>
constexpr Vec2<T>::Vec2(const Vec2<T>& other)
: Vec2(other.x, other.y) {}

template<typename T>
constexpr Vec2<T>::Vec2(T x, T y) : x(x), y(y) {}

template<typename T> template<typename U>
constexpr Vec2<T>::operator Vec2<U>() const {
  return Vec2<U>(x, y);
}

template<typename T> template<typename U,
  std::enable_if_t<VEC2_ENABLE_CONVERSION<T, U>, std::nullptr_t>>
constexpr Vec2<T>::Vec2(const U& other) : Vec2<T>(other.x, other.y) {}

template<typename T> template<typename U,
  std::enable_if_t<VEC2_ENABLE_CONVERSION<T, U>, std::nullptr_t>>
constexpr Vec2<T>::operator U() const {
  return U(x, y);
}

template<typename T>
constexpr Vec2<T>& Vec2<T>::operator =(const Vec2<T>& rhs) {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

template<typename T>
constexpr bool Vec2<T>::operator ==(const Vec2<T>& rhs) const {
  return (x == rhs.x) and (y == rhs.y);
}

template<typename T>
constexpr bool Vec2<T>::operator !=(const Vec2<T>& rhs) const {
  return (x != rhs.x) or (y != rhs.y);
}

template<typename T>
constexpr Vec2<T> Vec2<T>::operator +() const {
  return Vec2<T>(+x, +y);
}

template<typename T>
constexpr Vec2<T> Vec2<T>::operator +(const Vec2<T>& rhs) const {
  return Vec2<T>(x + rhs.x, y + rhs.y);
}

template<typename T>
constexpr Vec2<T> Vec2<T>::operator -() const {
  return Vec2<T>(-x, -y);
}

template<typename T>
constexpr Vec2<T> Vec2<T>::operator -(const Vec2<T>& rhs) const {
  return Vec2<T>(x - rhs.x, y - rhs.y);
}

template<typename T>
constexpr Vec2<T> Vec2<T>::operator *(const T& rhs) const {
  return Vec2<T>(x * rhs, y * rhs);
}

template<typename T>
constexpr Vec2<T> operator *(const T& lhs, const Vec2<T>& rhs) {
  return Vec2<T>(lhs * rhs.x, lhs * rhs.y);
}

template<typename T>
constexpr Vec2<T> operator/(const T& lhs, const Vec2<T>& rhs) {
  return Vec2<T>(lhs / rhs.x, lhs / rhs.y);
}

template<typename T>
constexpr Vec2<T> Vec2<T>::operator /(const T& rhs) const {
  return Vec2<T>(x / rhs, y / rhs);
}

template<typename T>
constexpr Vec2<T>& Vec2<T>::operator +=(const Vec2<T>& rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}

template<typename T>
constexpr Vec2<T>& Vec2<T>::operator -=(const Vec2<T>& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

template<typename T>
constexpr Vec2<T>& Vec2<T>::operator *=(const T& rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

template<typename T>
constexpr Vec2<T>& Vec2<T>::operator /=(const T& rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
}
}

template<>
struct std::hash<kme::Vec2<short>> {
  std::size_t operator()(const kme::Vec2<short>& key) const {
    // std::size_t should be at least 32 bits.
    static_assert(sizeof(std::size_t) >= 2 * sizeof(short),
                  "platform unsupported, word size too small");
    return key.x | key.y << 16;
  }
};

template<>
struct std::hash<kme::Vec2<int>> {
  std::size_t operator()(const kme::Vec2<int>& key) const {
    return key.x ^ (key.y << 16 | key.y >> 16);
  }
};
