#include "collision.hpp"

namespace kme {
bool WorldCollision::operator ==(const WorldCollision& rhs) const {
  return entity == rhs.entity and tile == rhs.tile;
}

bool EntityCollision::operator ==(const EntityCollision& rhs) const {
  return entity1 == rhs.entity1 and entity2 == rhs.entity2;
}
}
