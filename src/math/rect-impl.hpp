#pragma once

#include "rect-decl.hpp"

#include <type_traits>

namespace kme {
template<typename T>
constexpr Rect<T>::Rect()
: Rect(Vec2<T>(), Vec2<T>()) {}

template<typename T>
constexpr Rect<T>::Rect(const Rect<T>& rect)
: Rect(rect.pos, rect.size) {}

template<typename T>
constexpr Rect<T>::Rect(T x, T y, T width, T height)
: Rect(Vec2<T>(x, y), Vec2<T>(width, height)) {}

template<typename T>
constexpr Rect<T>::Rect(const Vec2<T>& pos, const Vec2<T>& size)
: pos(pos), size(size) {}

template<typename T> template<typename U>
constexpr Rect<T>::operator Rect<U>() const {
  return Rect<U>(static_cast<Vec2<U>>(pos), static_cast<Vec2<U>>(size));
}

template<typename T> template<typename U,
  std::enable_if_t<RECT_ENABLE_CONVERSION<T, U>, std::nullptr_t>>
constexpr Rect<T>::Rect(const U& other)
: Rect<T>(other.pos, other.size) {}

template<typename T> template<typename U,
  std::enable_if_t<RECT_ENABLE_CONVERSION<T, U>, std::nullptr_t>>
constexpr Rect<T>::operator U() const {
  return U(pos, size);
}

template<typename T>
constexpr Rect<T>& Rect<T>::operator =(const Rect<T>& rhs) {
  pos = rhs.pos;
  size = rhs.size;
  return *this;
}

template<typename T>
constexpr bool Rect<T>::operator ==(const Rect<T>& rhs) const {
  return pos == rhs.pos and size == rhs.size;
}

template<typename T>
constexpr bool Rect<T>::operator !=(const Rect<T>& rhs) const {
  return pos != rhs.pos or size != rhs.size;
}

template<typename T>
constexpr Rect<T> Rect<T>::operator &(const Rect<T>& rhs) const {
  return Rect<T>(
    std::max(x, rhs.x),
    std::max(y, rhs.y),
    x < rhs.x ? x + width - rhs.x : rhs.x + rhs.width - x,
    y < rhs.y ? y + height - rhs.y : rhs.y + rhs.height - y
  );
}
}
