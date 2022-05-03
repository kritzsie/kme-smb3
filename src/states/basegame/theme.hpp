#pragma once

#include "../../geometry.hpp"

#include <SFML/Graphics.hpp>

#include <string>

namespace kme {
struct Layer {
  std::string background;
  Vec2f offset;
  Vec2f parallax;
  bool repeat_y;
};

struct Theme {
  sf::Color background;
  std::map<int, Layer> layers;
  std::string music;
};
}
