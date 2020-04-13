#pragma once

#include "entitycomponentmanager.hpp"

namespace kme {
// begin Entity
class Entity {
public:
  Entity(const EntityComponentManager& entities,
         EntityComponentManager& entites_next,
         EntityID entity);

  template<typename T>
  const typename T::type& get() const;

  template<typename T>
  void set(const typename T::type& value);

  EntityID getID() const;

private:
  const EntityComponentManager& entities;
  EntityComponentManager& entities_next;
  EntityID entity;
};

template<typename T>
const typename T::type& Entity::get() const {
  return entities.get<T>(entity);
}

template<typename T>
void Entity::set(const typename T::type& value) {
  entities_next.get<T>(entity) = value;
}
// end Entity
}
