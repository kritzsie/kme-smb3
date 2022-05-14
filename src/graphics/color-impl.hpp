#include "color.hpp"

#include "../types.hpp"

#include <SFML/Graphics/Color.hpp>

#include <algorithm>

namespace kme {
constexpr Color::Color() : Color(0.f, 0.f, 0.f) {}

constexpr Color::Color(const Color& other)
: Color(other.r, other.g, other.b, other.a) {}

constexpr Color::Color(UInt32 color) : Color(
  ((color >> 24) & 0xFF) / 255.f,
  ((color >> 16) & 0xFF) / 255.f,
  ((color >> 8) & 0xFF) / 255.f,
  ((color >> 0) & 0xFF) / 255.f
) {}

constexpr Color::Color(float r, float g, float b, float a)
: r(r), g(g), b(b), a(a) {}

constexpr Color::Color(const sf::Color& other) :
  r(std::clamp(other.r / 255.f, 0.f, 1.f)),
  g(std::clamp(other.g / 255.f, 0.f, 1.f)),
  b(std::clamp(other.b / 255.f, 0.f, 1.f)),
  a(std::clamp(other.a / 255.f, 0.f, 1.f))
{}

constexpr Color& Color::operator =(const Color& rhs) {
  r = rhs.r;
  g = rhs.g;
  b = rhs.b;
  a = rhs.a;
  return *this;
}

constexpr bool Color::operator ==(const Color& rhs) const {
  return r == rhs.r and g == rhs.g and b == rhs.b and a == rhs.a;
}

constexpr bool Color::operator !=(const Color& rhs) const {
  return r != rhs.r or g != rhs.g or b != rhs.b or a != rhs.a;
}

constexpr Color Color::operator +() const {
  return Color(+r, +g, +b, +a);
}

constexpr Color Color::operator -() const {
  return Color(-r, -g, -b, -a);
}

constexpr Color Color::operator +(const Color& rhs) const {
  return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
}

constexpr Color Color::operator -(const Color& rhs) const {
  return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
}

constexpr Color Color::operator *(const Color& rhs) const {
  return Color(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
}

constexpr Color Color::operator /(const Color& rhs) const {
  return Color(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
}

constexpr Color Color::operator *(float rhs) const {
  return Color(r * rhs, g * rhs, b * rhs, a * rhs);
}

constexpr Color Color::operator /(float rhs) const {
  return Color(r / rhs, g / rhs, b / rhs, a / rhs);
}

constexpr Color operator *(float lhs, const Color& rhs) {
  return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
}

constexpr Color operator /(float lhs, const Color& rhs) {
  return Color(lhs / rhs.r, lhs / rhs.g, lhs / rhs.b, lhs / rhs.a);
}

constexpr Color& Color::operator +=(const Color& rhs) {
  r += rhs.r;
  g += rhs.g;
  b += rhs.b;
  a += rhs.a;
  return *this;
}

constexpr Color& Color::operator -=(const Color& rhs) {
  r -= rhs.r;
  g -= rhs.g;
  b -= rhs.b;
  a -= rhs.a;
  return *this;
}

constexpr Color& Color::operator *=(const Color& rhs) {
  r *= rhs.r;
  g *= rhs.g;
  b *= rhs.b;
  a *= rhs.a;
  return *this;
}

constexpr Color& Color::operator /=(const Color& rhs) {
  r /= rhs.r;
  g /= rhs.g;
  b /= rhs.b;
  a /= rhs.a;
  return *this;
}

constexpr Color& Color::operator *=(float rhs) {
  r *= rhs;
  g *= rhs;
  b *= rhs;
  a *= rhs;
  return *this;
}

constexpr Color& Color::operator /=(float rhs) {
  r /= rhs;
  g /= rhs;
  b /= rhs;
  a /= rhs;
  return *this;
}
}
