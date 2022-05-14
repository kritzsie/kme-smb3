#pragma once

#include "../types.hpp"

namespace sf {
class Color;
}

namespace kme {
struct Color {
  float r, g, b, a;

  constexpr Color();
  constexpr Color(const Color& other);
  constexpr Color(UInt32 color);
  constexpr Color(float r, float g, float b, float a = 1.f);

  constexpr Color(const sf::Color& other);
  operator sf::Color() const;

  constexpr Color& operator =(const Color& rhs);

  constexpr bool operator ==(const Color& rhs) const;
  constexpr bool operator !=(const Color& rhs) const;

  constexpr Color operator +() const;
  constexpr Color operator -() const;

  constexpr Color operator +(const Color& rhs) const;
  constexpr Color operator -(const Color& rhs) const;
  constexpr Color operator *(const Color& rhs) const;
  constexpr Color operator /(const Color& rhs) const;

  constexpr Color operator *(float rhs) const;
  constexpr Color operator /(float rhs) const;

  constexpr Color& operator +=(const Color& rhs);
  constexpr Color& operator -=(const Color& rhs);
  constexpr Color& operator *=(const Color& rhs);
  constexpr Color& operator /=(const Color& rhs);

  constexpr Color& operator *=(float rhs);
  constexpr Color& operator /=(float rhs);
};

constexpr Color operator *(float lhs, const Color& rhs);
constexpr Color operator /(float lhs, const Color& rhs);
}
