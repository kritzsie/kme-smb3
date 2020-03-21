#include "entitycomponents.hpp"

namespace kme {
Entity::Entity(EntityComponents& components, UInt32 index): components(components), index(index) {}

const EntityID& Entity::getId() const {
  return components.id.at(index);
}

const EntityType& Entity::getType() const {
  return components.type.at(index);
}

const Vec2f& Entity::getPos() const {
  return components.pos.at(index);
}

const Vec2f& Entity::getVel() const {
  return components.vel.at(index);
}

EntityID& Entity::getId() {
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
