#include "renderer.hpp"

#include "assetmanager.hpp"
#include "math.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

#include <string>

#include <cstddef>

namespace kme {
using namespace vec2_aliases;

TextStyle::TextStyle(std::string font_new, UInt32 flags_new) {
  flags = flags_new;
  font = font_new;
}

TextStyle::TextStyle(std::string font_new) : TextStyle(font_new, Flags::NONE) {}

void drawText(sf::RenderTarget* canvas, std::string text,
              Vec2f origin, TextStyle style) {
  const auto& texture = gfx.getTexture(style.font);
  Vec2u texture_size = texture.getSize();
  Vec2u view_size = canvas->getSize();
  std::size_t length = text.length();

  for (std::size_t i = 0; i < length; ++i) {
    UByte c = text.at(i);
    Rect<int> cliprect(c * 8 % texture_size.x, (c / 16) * 8 % texture_size.y, 8, 8);
    sf::Sprite sprite(texture, cliprect);
    bool align_right  = style.flags & TextStyle::Flags::ALIGN_RIGHT;
    bool align_bottom = style.flags & TextStyle::Flags::ALIGN_BOTTOM;
    Vec2f pos(
      align_right ? view_size.x - 8 * length - origin.x : origin.x,
      align_bottom ? view_size.y - 8 - origin.y : origin.y
    );
    sprite.setPosition(pos.x + 8 * i, pos.y);
    canvas->draw(sprite);
  }
}
}
