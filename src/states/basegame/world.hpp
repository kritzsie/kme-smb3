#pragma once

#include "entity.hpp"
#include "entitycomponentmanager.hpp"
#include "tilemap.hpp"

#include <unordered_map>

#include <cstdlib>

namespace kme {
// begin Subworld
class Subworld {
public:
  Entity spawnEntity(EntityType type, Vec2f pos);
  Entity getEntity(EntityID entity);

  const EntityComponentManager& getEntities() const;

  const Tilemap& getTiles() const;
  Tilemap& getTiles();

  void update(float delta_time);

private:
  EntityComponentManager entities;
  EntityComponentManager entities_next;

  Tilemap tiles;
};
// end Subworld

// begin Level
class Level {
public:
  using Map = std::unordered_map<std::size_t, Subworld>;

  using const_iterator = Map::const_iterator;
  using iterator = Map::iterator;

  Level();

  std::size_t createSubworld();
  std::size_t createSubworld(std::size_t index_hint);
  bool subworldExists(std::size_t index);
  bool deleteSubworld(std::size_t index);

  const Subworld& getSubworld(std::size_t index) const;
  Subworld& getSubworld(std::size_t index);

  const const_iterator cbegin() const;
  const const_iterator cend() const;
  const const_iterator begin() const;
  iterator begin();
  const const_iterator end() const;
  iterator end();

private:
  std::size_t counter = 0;

  Map subworld;
};
// end Level
}
