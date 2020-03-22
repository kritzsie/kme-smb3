#pragma once

#include "entitycomponents.hpp"
#include "tilemap.hpp"

#include <cstdlib>
#include <unordered_map>

namespace kme {
class Entity {
public:
  Entity(EntityComponents& entity_components,
         EntityComponents& entity_components_next,
         std::size_t index);

  const EntityID& getID() const;
  const EntityType& getType() const;
  const Vec2f& getPos() const;
  void setPos(Vec2f pos);
  const Vec2f& getVel() const;
  void setVel(Vec2f vel);

private:
  EntityComponents& entity_components;
  EntityComponents& entity_components_next;

  std::size_t index;
};

class Subworld {
public:
  Entity spawnEntity(EntityType type);

  const EntityComponents& getEntityComponents() const;

  const Tilemap& getTiles() const;
  Tilemap& getTiles();

  void update(float delta_time);

private:
  void expandEntityComponents();
  void shrinkEntityComponents();

  EntityID spawn_counter = 0;

  EntityComponents entity_components;
  EntityComponents entity_components_next;

  Tilemap tiles;
};

class Level {
public:
  using Map = std::unordered_map<UInt32, Subworld>;

  using const_iterator = Map::const_iterator;
  using iterator = Map::iterator;

  Level();

  UInt32 createSubworld();
  UInt32 createSubworld(UInt32 index_hint);
  bool subworldExists(UInt32 index);
  bool deleteSubworld(UInt32 index);

  const Subworld& getSubworld(UInt32 index) const;
  Subworld& getSubworld(UInt32 index);

  const const_iterator cbegin() const;
  const const_iterator cend() const;
  const const_iterator begin() const;
  iterator begin();
  const const_iterator end() const;
  iterator end();

private:
  UInt32 counter = 0;

  Map subworld;
};
}
