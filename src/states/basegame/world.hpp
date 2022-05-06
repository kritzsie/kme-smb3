#pragma once

#include "ecs/ecs.hpp"
#include "collision.hpp"
#include "theme.hpp"
#include "tilemap.hpp"

#include <map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include <cstdlib>

namespace kme {
class BaseGame;
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
  void setTiles(const Tilemap& tiles);

  Rect<int> getBounds() const;
  void setBounds(int width, int height);
  void setBounds(int x, int y, int width, int height);
  void setBounds(Rect<int> bounds);

  float getGravity() const;
  void setGravity(float value);

  std::string getTheme() const;
  void setTheme(std::string theme);

  void update(float delta);

private:
  void genEvent(EventType type, Event event);

  void genCollisionEvent(Entity entity, Vec2i tile);
  void genCollisionEvent(Entity entity1, Entity entity2);

  void consumeEvents();

  void checkWorldCollisions(Entity entity);
  void handleWorldCollisions(Entity entity);

  void checkEntityCollisions(Entity entity);
  void handleEntityCollisions(Entity entity);

public:
  Entity player;
  Entity camera;

private:
  const BaseGame* basegame;
  Gameplay* gameplay;

  EntityRegistry entities;
  Tilemap tiles;

  std::unordered_set<WorldCollision> world_collisions;
  std::unordered_set<EntityCollision> entity_collisions;

  Rect<int> bounds;
  float gravity = -60.f;

  std::string theme;
};
// end Subworld

// begin Level
class Level {
public:
  using Subworlds = std::map<std::size_t, Subworld>;

  Level(const BaseGame* basegame, Gameplay* gameplay);

  std::size_t createSubworld();
  std::size_t createSubworld(std::size_t index_hint);
  bool subworldExists(std::size_t index);
  bool deleteSubworld(std::size_t index);

  const Subworld& getSubworld(std::size_t index) const;
  Subworld& getSubworld(std::size_t index);

  using const_iterator = Subworlds::const_iterator;
  using iterator = Subworlds::iterator;

  const const_iterator cbegin() const;
  const const_iterator cend() const;
  const const_iterator begin() const;
  const const_iterator end() const;
  iterator begin();
  iterator end();

private:
  const BaseGame* basegame;
  Gameplay* gameplay;

  std::size_t count = 0;
  Subworlds subworlds;

public:
  float timer = 300.f;
};
// end Level
}
