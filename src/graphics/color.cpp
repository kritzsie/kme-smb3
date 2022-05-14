#include "color.hpp"

#include <algorithm>

namespace kme {
Color::operator sf::Color() const {
  return sf::Color(
    std::clamp(r * 255.f, 0.f, 255.f), std::clamp(g * 255.f, 0.f, 255.f),
    std::clamp(b * 255.f, 0.f, 255.f), std::clamp(a * 255.f, 0.f, 255.f)
  );
}
}
