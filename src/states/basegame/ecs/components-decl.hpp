#pragma once

#include "ecs.hpp"

#include "../../../geometry/vec2.hpp"

#include <type_traits>

namespace kme {
enum class Direction { RIGHT, LEFT };

struct FlagsComponent {
  enum : UInt32 {
    GRAVITY  = 1 << 0,
    AIRBORNE = 1 << 1,
    NODRAW   = 1 << 16
  };
};

struct PositionComponent : Component<Vec2f> {};

struct VelocityComponent : Component<Vec2f> {};

struct DirectionComponent : Component<Direction> {};

struct CollisionComponent {
  float radius;
  float height;
};

struct RenderComponent {
  Vec2f scale;
};
}
