#pragma once

#include "../../geometry.hpp"
#include "ecs/ecs.hpp"

#include <set>
#include <variant>

namespace kme {
struct WorldCollision {
  Entity subject;
  Vec2i tile;
};

struct EntityCollision {
  std::set<Entity> subjects;
};

struct CollisionEvent {
  enum class Type {
    WORLD, ENTITY
  };

  Type type;
  std::variant<WorldCollision, EntityCollision> collision;
};
}
