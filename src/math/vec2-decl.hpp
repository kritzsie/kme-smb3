#pragma once

#include "../types.hpp"

#include <type_traits>

namespace sf {
template<typename T>
class Vector2;
}

namespace kme {
template<typename T, typename U>
constexpr bool VEC2_ENABLE_CONVERSION = false;

template<typename T>
constexpr bool VEC2_ENABLE_CONVERSION<T, sf::Vector2<T>> = true;

template<typename T>
struct Vec2 {
  T x, y;

  constexpr Vec2();
  constexpr Vec2(T x, T y);

  constexpr bool operator ==(const Vec2<T>& rhs) const;
  constexpr bool operator !=(const Vec2<T>& rhs) const;

  constexpr Vec2<T> operator +() const;
  constexpr Vec2<T> operator -() const;
  constexpr Vec2<T> operator +(const Vec2<T>& rhs) const;
  constexpr Vec2<T> operator -(const Vec2<T>& rhs) const;
  constexpr Vec2<T> operator *(const T& rhs) const;
  constexpr Vec2<T> operator /(const T& rhs) const;

  constexpr Vec2<T>& operator +=(const Vec2<T>& rhs);
  constexpr Vec2<T>& operator -=(const Vec2<T>& rhs);
  constexpr Vec2<T>& operator *=(const T& rhs);
  constexpr Vec2<T>& operator /=(const T& rhs);

  template<typename U>
  constexpr explicit operator Vec2<U>() const;

  template<typename U,
    std::enable_if_t<VEC2_ENABLE_CONVERSION<T, U>, std::nullptr_t> = nullptr>
  constexpr Vec2(const U& other);
  template<typename U,
    std::enable_if_t<VEC2_ENABLE_CONVERSION<T, U>, std::nullptr_t> = nullptr>
  constexpr operator U() const;
};

template<typename T>
constexpr Vec2<T> operator *(const T& lhs, const Vec2<T>& rhs);

template<typename T>
constexpr Vec2<T> operator /(const T& lhs, const Vec2<T>& rhs);

namespace vec2_aliases {
  using Vec2s = Vec2<Int16>;
  using Vec2i = Vec2<Int32>;
  using Vec2u = Vec2<UInt32>;
  using Vec2l = Vec2<Int64>;
  using Vec2f = Vec2<float>;
  using Vec2d = Vec2<double>;
  using Vec2z = Vec2<std::size_t>;
}
}
