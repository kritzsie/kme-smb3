#pragma once

#include "ecs.hpp"

#include "../../../geometry.hpp"
#include "../../../input.hpp"
#include "../../../renderstates.hpp"
#include "../../../util.hpp"

#include <string>

namespace kme {
struct CDirection : Component<Sign> {};

struct CInfo {
  bool valid;
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

    MOVING     = 1 << 16,
    AIRBORNE   = 1 << 17
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
  Vec2f scale;
  RenderState state;
  float time;
};
}
