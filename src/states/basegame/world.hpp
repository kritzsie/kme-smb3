#pragma once

#include "ecs/ecs.hpp"
#include "ecs/entitydata.hpp"
#include "tilemap.hpp"

#include <unordered_map>

#include <cstdlib>

namespace kme {
// begin Subworld
class Subworld {
public:
  Subworld(const EntityData& entity_data, const TileDefs& tile_data);

  const EntityRegistry& getEntities() const;
  EntityRegistry& getEntities();

  const Tilemap& getTiles() const;
  Tilemap& getTiles();

  float getGravity();
  void setGravity(float value);

  void update(float delta);

  Entity player;
  Entity camera_target;

private:
  const EntityData& entity_data;
  const TileDefs& tile_data;

  EntityRegistry entities;
  Tilemap tiles;

  float gravity = -48.f;
};
// end Subworld

// begin Level
class Level {
public:
  using Map = std::unordered_map<std::size_t, Subworld>;

  using const_iterator = Map::const_iterator;
  using iterator = Map::iterator;

  Level(const EntityData& entity_data, const TileDefs& tile_data);

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
  const EntityData& entity_data;
  const TileDefs& tile_data;

  std::size_t counter = 0;

  Map subworld;
};
// end Level
}
