#include "entity.hpp"

namespace kme {
Entity::Entity(const EntityComponentManager& entities,
               EntityComponentManager& entities_next,
               EntityID entity)
: entities(entities), entities_next(entities_next), entity(entity) {}

EntityID Entity::getID() const {
  return entity;
}
}
