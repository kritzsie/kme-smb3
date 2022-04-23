#include "hitbox.hpp"

namespace kme {
Hitbox::Hitbox(float radius_new, float height_new)
: radius(radius_new), height(height_new) {}

Hitbox::Hitbox() : Hitbox(0.f, 0.f) {}

Rect<float> Hitbox::toAABB(Vec2f pos) const {
  return Rect<float>(pos.x + radius, pos.y, radius * 2, height);
}
}
