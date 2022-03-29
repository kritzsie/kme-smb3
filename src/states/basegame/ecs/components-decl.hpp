#pragma once

#include "ecs.hpp"
#include "../../../geometry/vec2.hpp"
#include "../../../input.hpp"

#include <entt/entt.hpp>

#include <string>

namespace kme {
enum class Direction { RIGHT, LEFT };

struct InfoComponent {
  std::string name;
};

struct FlagsComponent : Component<UInt32> {
  enum : UInt32 {
    NOGRAVITY  = 1 << 0,
    NOFRICTION = 1 << 1,
    AIRBORNE   = 1 << 16,
    CROUCHING  = 1 << 17,
    MOVING     = 1 << 24
  };
};

struct TimerComponent {
  float jump;
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
