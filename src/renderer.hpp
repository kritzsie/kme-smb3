#pragma once

#include "math.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

#include <string>

namespace kme {
using namespace vec2_aliases;

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

void drawText(sf::RenderTarget* canvas, std::string text,
              Vec2f origin, TextStyle style);
}
