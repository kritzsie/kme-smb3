#pragma once

#include "../../geometry.hpp"
#include "ecs/ecs.hpp"

#include <variant>

namespace kme {
struct WorldCollision {
  Entity entity;
  Vec2i tile;

  bool operator ==(const WorldCollision& rhs) const;
};

struct EntityCollision {
  Entity entity1;
  Entity entity2;

  bool operator ==(const EntityCollision& rhs) const;
};

struct CollisionEvent {
  enum class Type {
    WORLD, ENTITY
  };

  Type type;
  std::variant<WorldCollision, EntityCollision> collision;
};
}
