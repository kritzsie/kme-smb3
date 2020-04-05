#pragma once

#include "entitycomponentmanager.hpp"

namespace kme {
/*
// begin ComponentProxy
template<typename T>
class ComponentProxy {
public:
  ComponentProxy(const EntityComponentManager& entities,
                 EntityComponentManager& entities_next,
                 EntityID entity);

  operator typename T::type() const;

  ComponentProxy& operator =(const typename T::type& rhs);

private:
  const EntityComponentManager& entities;
  EntityComponentManager& entities_next;
  EntityID entity;
};

template<typename T>
ComponentProxy<T>::ComponentProxy(const EntityComponentManager& entities,
                                  EntityComponentManager& entities_next,
                                  EntityID entity)
: entities(entities), entities_next(entities_next), entity(entity) {}

template<typename T>
ComponentProxy<T>::operator typename T::type() const {
  return entities.get<T>(entity);
}

template<typename T>
ComponentProxy<T>& ComponentProxy<T>::operator =(const typename T::type& rhs) {
  entities_next.get<T>(entity) = rhs;
  return *this;
}
// end ComponentProxy
*/

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
