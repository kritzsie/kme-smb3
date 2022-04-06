#pragma once

#include "../../../geometry/vec2.hpp"
#include "../../../input.hpp"
#include "../../../renderstates.hpp"
#include "ecs.hpp"

#include <string>

namespace kme {
enum class Direction { RIGHT, LEFT };

template<typename T>
T operator *(const Direction& lhs, const T& rhs);

template<typename T>
T operator *(const T& lhs, const Direction& rhs);

struct DirectionComponent : Component<Direction> {};

struct InfoComponent {
  std::string type;
};

struct PositionComponent : Component<Vec2f> {};

struct VelocityComponent : Component<Vec2f> {};

struct FlagsComponent : Component<UInt32> {
  enum : UInt32 {
    FROZEN     = 1 << 0,
    NOGRAVITY  = 1 << 1,
    NOFRICTION = 1 << 2,
    AIRBORNE   = 1 << 16,
    CROUCHING  = 1 << 17,
    MOVING     = 1 << 24
  };
};

struct CollisionComponent {
  float radius;
  float height;
};

struct TimerComponent {
  float jump;
};

struct RenderComponent {
  Vec2f scale = Vec2f(1.f, 1.f);
  RenderState state;
};
}
