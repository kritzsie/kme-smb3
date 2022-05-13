#pragma once

#include "../../math.hpp"

namespace kme {
using namespace vec2_aliases;

class Hitbox {
public:
  float radius;
  float height;

  Hitbox();
  Hitbox(float radius, float height);

  Rect<float> toAABB(Vec2f pos) const;
};
}
