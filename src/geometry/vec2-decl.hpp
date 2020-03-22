#pragma once

#include "../types.hpp"

#include <SFML/System/Vector2.hpp>

#include <functional>

namespace kme {
template<typename T>
class Vec2 {
public:
  T x, y;

  Vec2();
  Vec2(T x, T y);
  Vec2(const Vec2<T>& vec2);
  Vec2(const sf::Vector2<T>& vec2);

  Vec2<T>& operator=(const Vec2<T>& rhs);

  Vec2<T> operator+() const;
  Vec2<T> operator-() const;
  Vec2<T> operator+(const Vec2<T>& rhs) const;
  Vec2<T> operator-(const Vec2<T>& rhs) const;
  Vec2<T> operator*(T rhs) const;
  Vec2<T> operator/(T rhs) const;

  bool operator==(const Vec2<T>& rhs) const;
  bool operator!=(const Vec2<T>& rhs) const;

  Vec2<T>& operator+=(const Vec2<T>& rhs);
  Vec2<T>& operator-=(const Vec2<T>& rhs);
  Vec2<T>& operator*=(T rhs);
  Vec2<T>& operator/=(T rhs);

  T dot(const Vec2<T>& other) const;
  T length() const;
  Vec2 normalized() const;

  Vec2 map(std::function<T (T)> f);

  template<typename U>
  explicit operator Vec2<U>() const;

  operator sf::Vector2<T>() const;
};

using Vec2i = Vec2<Int32>;
using Vec2f = Vec2<float>;
}
