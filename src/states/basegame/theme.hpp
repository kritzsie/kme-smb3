#pragma once

#include "../../graphics.hpp"
#include "../../math.hpp"

#include <string>

namespace kme {
using namespace vec2_aliases;

struct Layer {
  std::string background;
  Vec2f offset;
  Vec2f parallax;
  bool repeat_y;
};

struct Theme {
  Color background;
  std::map<int, Layer> layers;
  std::string music;
};
}
