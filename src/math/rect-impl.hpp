#pragma once

#include "rect-decl.hpp"

#include <algorithm>

namespace kme {
template<typename T>
Rect<T>::Rect() : pos(), size() {}

template<typename T>
Rect<T>::Rect(T x, T y, T width, T height) :
  pos(x, y), size(width, height)
{}

template<typename T>
Rect<T>::Rect(const Rect<T>& rect) :
  pos(rect.pos), size(rect.size)
{}

template<typename T>
Rect<T>::Rect(const Vec2<T>& pos, const Vec2<T>& size) :
  pos(pos), size(size)
{}

template<typename T>
Rect<T> Rect<T>::operator+() const {
  return Rect<T>(+x, +y, +width, +height);
}

template<typename T>
Rect<T> Rect<T>::operator-() const {
  return Rect<T>(-x, -y, -width, -height);
}

// Rect addition defined as the Minkowski sum of two rectangles
template<typename T>
Rect<T> Rect<T>::operator+(const Rect<T>& rhs) const {
  return Rect<T>(x + rhs.x, y + rhs.y, width + rhs.width, height + rhs.height);
}

// Likewise, subtraction defined as the Minkowski difference of two rectangles
template<typename T>
Rect<T> Rect<T>::operator-(const Rect<T>& rhs) const {
  return Rect<T>(x - rhs.x, y - rhs.y, width - rhs.width, height - rhs.height);
}

template<typename T>
Rect<T> Rect<T>::operator*(T rhs) const {
  return Rect<T>(x * rhs, y * rhs, width * rhs, height * rhs);
}

template<typename T>
Rect<T> operator*(T lhs, const Rect<T>& rhs) {
  return Rect<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.width, lhs * rhs.height);
}

template<typename T>
Rect<T> Rect<T>::operator/(T rhs) const {
  return Rect<T>(x / rhs, y / rhs, width / rhs, height / rhs);
}

template<typename T>
Rect<T> operator/(T lhs, const Rect<T>& rhs) {
  return Rect<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.width, lhs / rhs.height);
}

template<typename T>
bool Rect<T>::operator==(const Rect<T>& rhs) const {
  return (x == rhs.x) and
         (y == rhs.y) and
         (width == rhs.width) and
         (height == rhs.height);
}

template<typename T>
bool Rect<T>::operator!=(const Rect<T>& rhs) const {
  return (this->pos != rhs.pos) or (this->size != rhs.size);
}

template<typename T>
Rect<T>& Rect<T>::operator=(const Rect<T>& rhs) {
  x = rhs.x;
  y = rhs.y;
  width = rhs.width;
  height = rhs.height;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator+=(const Rect<T>& rhs) {
  x += rhs.x;
  y += rhs.y;
  width += rhs.width;
  height += rhs.height;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator-=(const Rect<T>& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  width -= rhs.width;
  height -= rhs.height;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator*=(T rhs) {
  x *= rhs;
  y *= rhs;
  width *= rhs;
  height *= rhs;
  return *this;
}

template<typename T>
Rect<T>& Rect<T>::operator/=(T rhs) {
  x /= rhs;
  y /= rhs;
  width /= rhs;
  height /= rhs;
  return *this;
}

template<typename T>
bool Rect<T>::intersects(const Rect<T>& rhs) const {
  return x < rhs.x + rhs.width
  and    x + width > rhs.x
  and    y < rhs.y + rhs.height
  and    y + height > rhs.y;
}

template<typename T>
Rect<T> Rect<T>::intersection(const Rect<T>& rhs) const {
  return Rect<T>(
    std::max(x, rhs.x),
    std::max(y, rhs.y),
    x < rhs.x ? x + width  - rhs.x : rhs.x + rhs.width  - x,
    y < rhs.y ? y + height - rhs.y : rhs.y + rhs.height - y
  );
}

template<typename T>
Vec2<T> Rect<T>::midpoint() const {
  const Vec2<T> r = radius();
  return Vec2<T>(x + r.x, y + r.y);
}

template<typename T>
Vec2<T> Rect<T>::radius() const {
  return Vec2<T>(width / 2, height / 2);
}

template<typename T>
Rect<T> Rect<T>::map(std::function<T (T)> f) {
  return Rect<T>(f(x), f(y), f(width), f(height));
}

template<typename T> template<typename U>
Rect<T>::operator Rect<U>() const {
  return Rect<U>(x, y, width, height);
}

template<typename T>
Rect<T>::operator sf::Rect<T>() const {
  return sf::Rect<T>(x, y, width, height);
}
}
