#pragma once

#include "../basegame.hpp"
#include "ecs/ecs.hpp"
#include "collision.hpp"
#include "theme.hpp"
#include "tilemap.hpp"

#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <cstdlib>

namespace kme {
class Gameplay;

// begin Subworld
class Subworld {
public:
  enum class EventType {
    COLLISION
  };

  using Event = std::variant<CollisionEvent>;

  Subworld(const BaseGame* basegame, Gameplay* gameplay);

  const EntityRegistry& getEntities() const;
  EntityRegistry& getEntities();

  const Tilemap& getTiles() const;
  Tilemap& getTiles();

  Rect<int> getBounds() const;
  void setBounds(int width, int height);
  void setBounds(int x, int y, int width, int height);
  void setBounds(Rect<int> size);

  float getGravity() const;
  void setGravity(float value);

  std::string getTheme() const;
  void setTheme(std::string theme);

  void genEvent(EventType type, Event event);

  void update(float delta);

private:
  void consumeCollisionEvents();

public:
  Entity player;
  Entity camera;

private:
  const BaseGame* basegame;
  Gameplay* gameplay;

  EntityRegistry entities;
  Tilemap tiles;

  std::vector<WorldCollision> world_collisions;
  std::vector<EntityCollision> entity_collisions;

  Rect<int> bounds;
  float gravity = -60.f;

  std::string theme;
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

public:
  float timer = 300.f;
};
// end Level
}
