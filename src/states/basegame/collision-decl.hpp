#pragma once

#include "../../math.hpp"
#include "ecs/ecs.hpp"
#include "tilemap.hpp"

#include <variant>

namespace kme {
using namespace vec2_aliases;

struct WorldCollision {
  Entity entity;
  Tile tile;

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
