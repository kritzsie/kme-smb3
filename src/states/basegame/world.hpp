#pragma once

#include "ecs/ecs.hpp"
#include "tilemap.hpp"
#include "../basegame.hpp"

#include <unordered_map>

#include <cstdlib>

namespace kme {
class Gameplay;

// begin Subworld
class Subworld {
public:
  Subworld(const BaseGame* basegame, Gameplay* gameplay);

  const EntityRegistry& getEntities() const;
  EntityRegistry& getEntities();

  const Tilemap& getTiles() const;
  Tilemap& getTiles();

  Rect<int> getSize() const;
  void setSize(int width, int height);
  void setSize(int x, int y, int width, int height);
  void setSize(Rect<int> size);

  float getGravity() const;
  void setGravity(float value);

  void update(float delta);

  Entity player;
  Entity camera;

private:
  const BaseGame* basegame;
  Gameplay* gameplay;

  EntityRegistry entities;
  Tilemap tiles;

  Rect<int> size;
  float gravity = -60.f;
};
// end Subworld

// begin Level
class Level {
public:
  using Map = std::unordered_map<std::size_t, Subworld>;

  using const_iterator = Map::const_iterator;
  using iterator = Map::iterator;

  Level(const BaseGame* basegame, Gameplay* gameplay);

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
  const BaseGame* basegame;
  Gameplay* gameplay;

  std::size_t count = 0;

  Map subworld;
};
// end Level
}
