#pragma once

#include "../../geometry/rect.hpp"

namespace kme {
class Hitbox {
public:
  float radius;
  float height;

  Hitbox();
  Hitbox(float radius, float height);

  Rect<float> toAABB(Vec2f pos) const;
};
}
