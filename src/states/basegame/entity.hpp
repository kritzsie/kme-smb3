#pragma once

#include "entitycomponentmanager.hpp"
#include "world.hpp"

namespace kme {
/*
// begin Entity
class Entity {
public:
  Entity(Subworld& subworld,
         EntityID entity);

  template<typename T>
  const typename T::ComponentType& get() const;

  template<typename T>
  void set(const typename T::ComponentType& value);

  EntityID getID() const;

  Entity spawnEntity(EntityType type);

private:
  Subworld& subworld;

  EntityID entity;
};

template<typename T>
const typename T::ComponentType& Entity::get() const {
  return static_cast<const Subworld&>(subworld).getEntities().get<T>(entity);
}

template<typename T>
void Entity::set(const typename T::ComponentType& value) {
  subworld.getEntities().get<T>(entity) = value;
}
// end Entity
*/
}
