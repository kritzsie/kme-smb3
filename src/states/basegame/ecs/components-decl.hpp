#pragma once

#include "ecs.hpp"

#include "../../../geometry/vec2.hpp"

#include <entt/entt.hpp>

#include <string>

namespace kme {
enum class Direction { RIGHT, LEFT };

struct InfoComponent {
  enum {
    NOGRAVITY  = 1 << 0,
    NOFRICTION = 1 << 1,
    AIRBORNE   = 1 << 16,
    MOVING     = 1 << 17
  };

  std::string name;
  UInt32 flags;
};

struct PositionComponent : Component<Vec2f> {};

struct VelocityComponent : Component<Vec2f> {};

struct DirectionComponent : Component<Direction> {};

struct CollisionComponent {
  float radius;
  float height;
};

struct RenderComponent {
  enum : UInt32 {
    NODRAW = 1 << 0
  };

  UInt32 flags;
  Vec2f scale;
};

struct SpawnerComponent {
  entt::entity child;
};
}
