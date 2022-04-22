#pragma once

#include "geometry.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

namespace kme {
class TextStyle {
public:
  struct Flags {
    enum : UInt32 {
      NONE = 0,
      ALIGN_RIGHT  = 1 << 0,
      ALIGN_BOTTOM = 1 << 1
    };
  };

  TextStyle(std::string font);
  TextStyle(std::string font, UInt32 flags);

  UInt32 flags;
  std::string font;
};

void drawText(sf::RenderTarget* canvas, std::string text, Vec2f pos, TextStyle style);
}
