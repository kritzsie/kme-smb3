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
  Vec2f scale = Vec2f(1.f, 1.f);
  RenderState state;
};
}
