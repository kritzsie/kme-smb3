#pragma once

#include "../../geometry.hpp"

#include <SFML/Graphics.hpp>

#include <string>

namespace kme {
struct Layer {
  std::string texture;
  Vec2f offset;
  Vec2f parallax;
  bool repeat_y;
};

struct Style {
  sf::Color background;
  std::map<int, Layer> layers;
  std::string music;
};
}
