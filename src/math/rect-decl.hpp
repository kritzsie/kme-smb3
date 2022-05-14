#pragma once

#include "vec2.hpp"

#include <type_traits>

namespace sf {
template<typename T>
class Rect;
}

namespace kme {
template<typename T, typename U>
constexpr bool RECT_ENABLE_CONVERSION = false;

template<typename T>
constexpr bool RECT_ENABLE_CONVERSION<T, sf::Rect<T>> = true;

template<typename T>
struct Rect {
  Vec2<T> pos, size;

  T& x = pos.x;
  T& y = pos.y;
  T& width = size.x;
  T& height = size.y;

  constexpr Rect();
  constexpr Rect(const Rect& rect);
  constexpr Rect(T x, T y, T width, T height);
  constexpr Rect(const Vec2<T>& pos, const Vec2<T>& size);

  template<typename U>
  constexpr explicit operator Rect<U>() const;

  template<typename U,
    std::enable_if_t<RECT_ENABLE_CONVERSION<T, U>, std::nullptr_t> = nullptr>
  constexpr Rect(const U& other);
  template<typename U,
    std::enable_if_t<RECT_ENABLE_CONVERSION<T, U>, std::nullptr_t> = nullptr>
  constexpr operator U() const;

  constexpr Rect& operator =(const Rect&);

  constexpr bool operator ==(const Rect& rhs) const;
  constexpr bool operator !=(const Rect& rhs) const;

  constexpr Rect operator &(const Rect& rhs) const;
};
}
