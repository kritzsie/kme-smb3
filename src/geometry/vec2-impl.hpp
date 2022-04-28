#pragma once

#include "vec2-decl.hpp"

#include <functional>

namespace kme {
template<typename T>
Vec2<T>::Vec2() : x(), y() {}

template<typename T>
Vec2<T>::Vec2(T x, T y) : x(x), y(y) {}

template<typename T>
Vec2<T>::Vec2(const Vec2<T>& vec2) : x(vec2.x), y(vec2.y) {}

template<typename T>
Vec2<T>::Vec2(const sf::Vector2<T>& vec2) : Vec2<T>(vec2.x, vec2.y) {}

template<typename T>
Vec2<T>& Vec2<T>::operator=(const Vec2<T>& rhs) {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

template<typename T>
Vec2<T> Vec2<T>::operator+() const {
  return Vec2<T>(+x, +y);
}

template<typename T>
Vec2<T> Vec2<T>::operator+(const Vec2<T>& rhs) const {
  return Vec2<T>(x + rhs.x, y + rhs.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator-() const {
  return Vec2<T>(-x, -y);
}

template<typename T>
Vec2<T> Vec2<T>::operator-(const Vec2<T>& rhs) const {
  return Vec2<T>(x - rhs.x, y - rhs.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator*(const T rhs) const {
  return Vec2<T>(x * rhs, y * rhs);
}

template<typename T>
Vec2<T> operator*(T lhs, const Vec2<T>& rhs) {
  return Vec2<T>(lhs * rhs.x, lhs * rhs.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator/(T rhs) const {
  return Vec2<T>(x / rhs, y / rhs);
}

template<typename T>
Vec2<T> operator/(T lhs, const Vec2<T>& rhs) {
  return Vec2<T>(lhs / rhs.x, lhs / rhs.y);
}

template<typename T>
bool Vec2<T>::operator==(const Vec2<T>& rhs) const {
  return (x == rhs.x) and (y == rhs.y);
}

template<typename T>
bool Vec2<T>::operator!=(const Vec2<T>& rhs) const {
  return (x != rhs.x) or (y != rhs.y);
}

template<typename T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator*=(T rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

template<typename T>
Vec2<T>& Vec2<T>::operator/=(T rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
}

template<typename T>
T Vec2<T>::dot(const Vec2<T>& other) const {
  return acos(x * other.x + y * other.y);
}

template<typename T>
T Vec2<T>::length() const {
  return sqrt(x * x + y * y);
}

template<typename T>
Vec2<T> Vec2<T>::normalized() const {
  return *this / length();
}

template<typename T>
Vec2<T> Vec2<T>::map(std::function<T (T)> f) {
  return Vec2<T>(f(x), f(y));
}

template<typename T>
Vec2<T>::operator sf::Vector2<T>() const {
  return sf::Vector2<T>(x, y);
}

template<typename T> template<typename U>
Vec2<T>::operator Vec2<U>() const {
  return Vec2<U>(x, y);
}
}

template<>
struct std::hash<kme::Vec2<short>> {
  std::size_t operator()(const kme::Vec2<short>& key) const {
    // std::size_t should be at least 32 bits.
    static_assert(sizeof(std::size_t) >= 2 * sizeof(short), "platform unsupported, word size too small");
    return key.x | key.y << 16;
  }
};

template<>
struct std::hash<kme::Vec2<int>> {
  std::size_t operator()(const kme::Vec2<int>& key) const {
    return key.x ^ (key.y << 16 | key.y >> 16);
  }
};
