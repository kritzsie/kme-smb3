#pragma once

#include "vec2.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace kme {
template<typename T>
class Rect {
public:
  T x, y;
  T width, height;

  Rect<T> operator+() const;
  Rect<T> operator-() const;
  Rect<T> operator+(const Rect<T>&) const;
  Rect<T> operator-(const Rect<T>&) const;
  Rect<T> operator*(T) const;
  Rect<T> operator/(T) const;

  Rect<T> operator+(const Vec2<T>&) const;
  Rect<T> operator-(const Vec2<T>&) const;

  bool operator==(const Rect<T>&) const;
  bool operator!=(const Rect<T>&) const;

  Rect<T>& operator=(const Rect<T>&);
  Rect<T>& operator+=(const Rect<T>&);
  Rect<T>& operator-=(const Rect<T>&);
  Rect<T>& operator*=(T);
  Rect<T>& operator/=(T);

  operator sf::Rect<T>() const;

  Vec2<T> midpoint() const;
  bool intersects(const Rect<T>&) const;
  Rect<T> intersection(const Rect<T>&) const;

  Rect();
  Rect(T);
  Rect(T, T, T, T);
  Rect(const Rect<T>&);
  Rect(const Vec2<T>&, const Vec2<T>&);
};
}
