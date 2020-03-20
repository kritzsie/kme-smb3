#include "entities.hpp"

namespace kme {
Entity::Entity(Entities& entities, UInt32 index) : entities(entities), index(index) {}

const UInt32& Entity::getId() const {
  return entities.id.at(index);
}

const EntityType& Entity::getType() const {
  return entities.type.at(index);
}

const Vec2f& Entity::getPos() const {
  return entities.pos.at(index);
}

const Vec2f& Entity::getVel() const {
  return entities.vel.at(index);
}

UInt32& Entity::getId() {
  return const_cast<UInt32&>(static_cast<const Entity*>(this)->getId());
}

EntityType& Entity::getType() {
  return const_cast<EntityType&>(static_cast<const Entity*>(this)->getType());
}

Vec2f& Entity::getPos() {
  return const_cast<Vec2f&>(static_cast<const Entity*>(this)->getPos());
}

Vec2f& Entity::getVel() {
  return const_cast<Vec2f&>(static_cast<const Entity*>(this)->getVel());
}
}
