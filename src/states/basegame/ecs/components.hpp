#pragma once

#include "ecs.hpp"

#include "../../../geometry.hpp"
#include "../../../input.hpp"
#include "../../../renderstates.hpp"
#include "../../../sound.hpp"
#include "../../../util.hpp"

#include <entt/entt.hpp>

#include <string>

#include <cstddef>

namespace kme {
struct CDirection : Component<Sign> {};

struct CInfo {
  bool valid = true;
  std::string type;
  Entity parent = entt::null;
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

    AIRBORNE   = 1 << 16
  };
};

struct CFlags : Component<UInt32> {};

struct CCollision {
  float radius = 8.f;
  float height = 16.f;
};

struct CTimer {
  float jump = 0.f;
};

enum class EState {
  IDLE, DEAD, DOOR, PIPE,
  WALK, RUN, SLIP, DUCK,
  AIRBORNE, RUNJUMP, UNDERWATER, SWIM
};

struct CState : Component<EState> {};

struct CRender {
  Vec2f scale = Vec2f(1.f, 1.f);
  RenderState state;
  float time = 0.f;
};

struct CAudio {
  struct Channels {
    std::size_t slip;
    std::size_t speed;
  } channels {Sound::MAX_VOICES};
};
}
