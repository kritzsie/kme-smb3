#pragma once

#include "../../geometry/vec2.hpp"
#include "../../types.hpp"

#include <set>
#include <string>
#include <tuple>
#include <unordered_map>

#include <cstdlib>

namespace kme {
using EntityID = std::size_t;
using EntityType = std::string;

template<typename T>
struct Component {
  using type = T;

  std::unordered_map<EntityID, T> value;
};

struct Box {
  float radius;
  float height;
};

struct Type : Component<EntityType> {};
struct Flags : Component<UInt32> {
  enum : UInt32 {
    NONE      = 0,
    GRAVITY   = 1 << 0,
    LANDED    = 1 << 1
  };
};
struct Position : Component<Vec2f> {};
struct Velocity : Component<Vec2f> {};
struct CollisionBox : Component<Box> {};

using ComponentTypes = std::tuple<
  Type,
  Flags,
  Position,
  Velocity,
  CollisionBox
>;

class EntityComponentManager {
public:
  static constexpr std::size_t DEFAULT_CAPACITY = 32;

  using Set = std::set<EntityID>;

  using const_iterator = Set::const_iterator;
  using const_reverse_iterator = Set::const_reverse_iterator;
  using iterator = Set::iterator;
  using reverse_iterator = Set::reverse_iterator;

  EntityComponentManager() noexcept;

  template<typename T>
  const typename T::type& get(EntityID entity) const;
  template<typename T>
  typename T::type& get(EntityID entity);

  template<typename T>
  void set(EntityID entity, const typename T::type& value) noexcept;

  std::size_t getCapacity() const noexcept;
  void setCapacity(std::size_t requested);

  EntityID createEntity() noexcept;
  void removeEntity(EntityID entity);

  const const_iterator cbegin() const;
  const const_iterator cend() const;
  const const_iterator begin() const;
  iterator begin();
  const const_iterator end() const;
  iterator end();

  const const_reverse_iterator crbegin() const;
  const const_reverse_iterator crend() const;
  const const_reverse_iterator rbegin() const;
  reverse_iterator rbegin();
  const const_reverse_iterator rend() const;
  reverse_iterator rend();

private:
  std::size_t capacity = 0;
  std::size_t slot = 0;

  Set entities;
  ComponentTypes components;
};

template<typename T>
const typename T::type& EntityComponentManager::get(EntityID entity) const {
  return std::get<T>(components).value.at(entity);
}

template<typename T>
typename T::type& EntityComponentManager::get(EntityID entity) {
  return const_cast<typename T::type&>(static_cast<const EntityComponentManager*>(this)->get<T>(entity));
}

template<typename T>
void EntityComponentManager::set(EntityID entity, const typename T::type& value) noexcept {
  std::get<T>(components).value.at(entity) = value;
}
}
