#pragma once

#include "ecs.hpp"

#include "../../../input.hpp"
#include "../../../math.hpp"
#include "../../../renderstates.hpp"
#include "../../../sound.hpp"
#include "../../../util.hpp"
#include "../hitbox.hpp"
#include "../powerup.hpp"
#include "../states.hpp"
#include "../tilemap.hpp"

#include <entt/entt.hpp>

#include <string>
#include <unordered_set>

#include <cstddef>

namespace kme {
using namespace vec2_aliases;

struct EFlags {
  enum : UInt32 {
    DEAD       = 1 << 0,
    WALKING    = 1 << 1,
    RUNNING    = 1 << 2,
    DUCKING    = 1 << 3,
    FROZEN     = 1 << 4,
    NOGRAVITY  = 1 << 5,
    NOFRICTION = 1 << 6,
    NOCLIP     = 1 << 7,

    AIRBORNE   = 1 << 8,
    UNDERWATER = 1 << 9,
    LANDED     = 1 << 12,

    SOLID      = 1 << 16,
    INTANGIBLE = 1 << 17,
    ENEMY      = 1 << 18,
    POWERUP    = 1 << 19
  };
};

struct CInfo {
  std::string type;
  Entity parent = entt::null;
  bool valid = true;
};

struct CPosition {
  Vec2f value;
};

struct CVelocity {
  Vec2f value;
};

struct CDirection {
  Sign value;
};

struct CFlags {
  UInt32 value;
};

struct CPowerup {
  Powerup value;
};

struct CState {
  EState value;
};

struct CCollision {
  Hitbox hitbox;
  Vec2f pos_old;
  std::unordered_set<Tile> tiles;
  std::unordered_set<Entity> entities;
};

struct CCounters {
  float p_meter = 0.f;
};

struct CTimers {
  float death = 0.f;
  float i_frames = 0.f;
  float jump = 0.f;
  float p_speed = 0.f;
  float swim = 0.f;
};

struct CRender {
  RenderState state;
  float time = 0.f;
  Vec2f scale = Vec2f(1.f, 1.f);
};

struct CAudio {
  struct Channels {
    std::size_t slip;
    std::size_t speed;
  } channels {Sound::MAX_VOICES};
};
}
