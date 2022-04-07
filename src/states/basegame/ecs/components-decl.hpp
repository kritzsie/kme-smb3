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

struct CDirection : Component<Direction> {};

struct CInfo {
  std::string type;
};

struct CPosition : Component<Vec2f> {};

struct CVelocity : Component<Vec2f> {};

struct EFlags {
  enum : UInt32 {
    DEAD       = 1 << 0,
    WALKING    = 1 << 1,
    RUNNING    = 1 << 2,
    DUCKING    = 1 << 3,
    FROZEN     = 1 << 4,
    NOGRAVITY  = 1 << 5,
    NOFRICTION = 1 << 6,
    AIRBORNE   = 1 << 16,
    CROUCHING  = 1 << 17,
    MOVING     = 1 << 24
  };
};

struct CFlags : Component<UInt32> {};

struct CCollision {
  float radius;
  float height;
};

struct CTimer {
  float jump;
};

enum class EState {
  IDLE, DEAD, DOOR, PIPE,
  WALKING, RUNNING, SLIPPING, DUCKING,
  AIRBORNE, RUNJUMPING, UNDERWATER, SWIMMING
};

struct CState : Component<EState> {};

struct CRender {
  Vec2f scale = Vec2f(1.f, 1.f);
  RenderState state;
  float time;
};
}
