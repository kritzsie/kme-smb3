#include "renderer.hpp"

#include "assetmanager.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>

#include <cstddef>

namespace kme {
TextStyle::TextStyle(std::string font_new, UInt32 flags_new) {
  flags = flags_new;
  font = font_new;
}

TextStyle::TextStyle(std::string font_new) : TextStyle(font_new, Flags::NONE) {}

void drawText(sf::RenderTarget* canvas, std::string text, Vec2f pos, TextStyle style) {
  const auto& texture = gfx.getTexture(style.font);
  sf::Vector2u viewsize = canvas->getSize();
  std::size_t length = text.size();

  for (std::size_t i = 0; i < length; ++i) {
    UByte c = text.at(i);
    sf::IntRect cliprect(
      c * 8 % texture.getSize().x, (c / 16) * 8 % texture.getSize().y, 8, 8
    );
    sf::Sprite sprite(texture, cliprect);
    bool align_right  = style.flags & TextStyle::Flags::ALIGN_RIGHT;
    bool align_bottom = style.flags & TextStyle::Flags::ALIGN_BOTTOM;
    float x_pos = align_right ? viewsize.x - 8.f * length - pos.x : pos.x;
    float y_pos = align_bottom ? viewsize.y - 8.f - pos.y : pos.y;
    sprite.setPosition(x_pos + 8.f * i, y_pos);
    canvas->draw(sprite);
  }
}
}
