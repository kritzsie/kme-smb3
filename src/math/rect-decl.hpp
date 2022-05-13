#pragma once

#include "vec2.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <functional>

namespace kme {
template<typename T>
class Rect {
public:
  Vec2<T> pos, size;

  T& x = pos.x;
  T& y = pos.y;
  T& width = size.x;
  T& height = size.y;

  Rect();
  Rect(T x, T y, T width, T height);
  Rect(const Rect<T>& rect);
  Rect(const Vec2<T>& pos, const Vec2<T>& size);
  Rect(const sf::Rect<T>& rect);

  Rect<T>& operator=(const Rect<T>&);

  Rect<T> operator+() const;
  Rect<T> operator-() const;
  Rect<T> operator+(const Rect<T>& rhs) const;
  Rect<T> operator-(const Rect<T>& rhs) const;
  Rect<T> operator*(T rhs) const;
  Rect<T> operator/(T rhs) const;

  Rect<T> operator+(const Vec2<T>& rhs) const;
  Rect<T> operator-(const Vec2<T>& rhs) const;

  bool operator==(const Rect<T>& rhs) const;
  bool operator!=(const Rect<T>& rhs) const;

  Rect<T>& operator+=(const Rect<T>& rhs);
  Rect<T>& operator-=(const Rect<T>& rhs);
  Rect<T>& operator*=(T rhs);
  Rect<T>& operator/=(T rhs);

  bool intersects(const Rect<T>& other) const;
  Rect<T> intersection(const Rect<T>& other) const;

  Vec2<T> midpoint() const;
  Vec2<T> radius() const;

  Rect<T> map(std::function<T (T)> f);

  template<typename U>
  explicit operator Rect<U>() const;

  operator sf::Rect<T>() const;
};
}
