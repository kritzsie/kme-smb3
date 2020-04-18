#include "world.hpp"

#include <exception>

#include <ctgmath>

namespace kme {
// begin Subworld
Subworld::Subworld(const EntityData& entity_data, const TileDefs& tile_data)
: entity_data(entity_data), tile_data(tile_data) {}

Entity Subworld::spawnEntity(EntityType type, Vec2f pos) {
  Entity entity = getEntity(entities_next.createEntity());
  entity.set<Type>(type);
  entity.set<Position>(pos);

  try {
    entity.set<CollisionBox>(entity_data.getCollisionBox(type));
  }
  catch (const std::out_of_range& ex) {}

  return entity;
}

Entity Subworld::getEntity(EntityID entity) {
  return Entity(entities, entities_next, entity);
}

const EntityComponentManager& Subworld::getEntities() const {
  return entities;
}

const Tilemap& Subworld::getTiles() const {
  return tiles;
}

Tilemap& Subworld::getTiles() {
  return const_cast<Tilemap&>(static_cast<const Subworld*>(this)->getTiles());
}

// ugly
static Rect<float> toAABB(Vec2f pos, Box box) {
  return Rect<float>(pos.x - box.radius, pos.y, box.radius * 2.f, box.height);
}

// ugly
static Rect<int> toRange(Rect<float> aabb) {
  return Rect<int>(
    std::floor(aabb.x), std::floor(aabb.y),
    std::ceil(aabb.width + 1.f), std::ceil(aabb.height + 1.f)
  );
}

void Subworld::update(float delta) {
  // apply gravity
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    Entity entity = getEntity(*iter);
    if (entity.get<Flags>() & Flags::GRAVITY) {
      Vec2f vel = entity.get<Velocity>();
      vel.y -= 29.625f * delta;
      entity.set<Velocity>(vel);
    }
  }

  commit_entities();

  // move and collide
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    // y axis
    Entity entity = getEntity(*iter);
    Vec2f pos = entity.get<Position>();
    Vec2f vel = entity.get<Velocity>();

    pos.x += vel.x * delta;

    Rect<float> entity_aabb = toAABB(pos, entity.get<CollisionBox>());
    Rect<int> range = toRange(entity_aabb);
    for (int y = range.y; y < range.y + range.height; ++y)
    for (int x = range.x; x < range.x + range.width;  ++x) {
      Rect<float> tile_aabb = Rect<float>(x, y, 1.f, 1.f);
      if (entity_aabb.intersects(tile_aabb)) {
        switch (tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
        case TileDef::CollisionType::SOLID: {
          Rect<float> collision = entity_aabb.intersection(tile_aabb);
          if (entity_aabb.x > x + 0.5f) {
            pos.x += collision.width;
          }
          else {
            pos.x -= collision.width;
          }
          vel.x = 0.f;
          entity_aabb = toAABB(pos, entity.get<CollisionBox>());
          break;
        }
        case TileDef::CollisionType::NONE:
        default:
          break;
        }
      }
    }

    bool landed = false;

    pos.y += vel.y * delta;

    entity_aabb = toAABB(pos, entity.get<CollisionBox>());
    range = toRange(entity_aabb);
    for (int y = range.y; y < range.y + range.height; ++y)
    for (int x = range.x; x < range.x + range.width;  ++x) {
      Rect<float> tile_aabb = Rect<float>(x, y, 1.f, 1.f);
      if (entity_aabb.intersects(tile_aabb)) {
        switch (tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
        case TileDef::CollisionType::SOLID: {
          Rect<float> collision = entity_aabb.intersection(tile_aabb);
          if (entity_aabb.y > y + 0.5f) {
            landed = true;
            pos.y += collision.height;
          }
          else {
            pos.y -= collision.height;
          }
          vel.y = 0.f;
          entity_aabb = toAABB(pos, entity.get<CollisionBox>());
          break;
        }
        case TileDef::CollisionType::NONE:
        default:
          break;
        }
      }
    }

    if (landed) entity.set<Flags>(entity.get<Flags>() |  Flags::LANDED);
    else        entity.set<Flags>(entity.get<Flags>() & ~Flags::LANDED);

    entity.set<Position>(pos);
    entity.set<Velocity>(vel);
  }

  commit_entities();

  // update render states
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
  }

  commit_entities();
}

void Subworld::commit_entities() {
  entities = entities_next;
}
// end Subworld

// begin Level
Level::Level(const EntityData& entity_data, const TileDefs& tile_data)
: entity_data(entity_data), tile_data(tile_data) {
  createSubworld();
}

std::size_t Level::createSubworld() {
  return createSubworld(counter++);
}

// TODO: de-duplicate this
std::size_t Level::createSubworld(std::size_t index_hint) {
  if (subworld.find(index_hint) == subworld.end()) {
    if (counter == index_hint) {
      ++counter;
    }

    subworld.emplace(index_hint, Subworld(entity_data, tile_data));
    return index_hint;
  }

  while (subworld.find(counter) != subworld.end()) {
    ++counter;
  }

  subworld.emplace(counter, Subworld(entity_data, tile_data));
  return counter;
}

bool Level::subworldExists(std::size_t index) {
  return index == 0 or subworld.find(index) != subworld.end();
}

bool Level::deleteSubworld(std::size_t index) {
  if (subworldExists(index)) {
    subworld.erase(index);
    return true;
  }

  return false;
}

const Subworld& Level::getSubworld(std::size_t index) const {
  return subworld.at(index);
}

Subworld& Level::getSubworld(std::size_t index) {
  return const_cast<Subworld&>(static_cast<const Level*>(this)->getSubworld(index));
}

const Level::const_iterator Level::begin() const {
  return subworld.cbegin();
}

const Level::const_iterator Level::end() const {
  return subworld.cend();
}

const Level::const_iterator Level::cbegin() const {
  return begin();
}

const Level::const_iterator Level::cend() const {
  return end();
}

Level::iterator Level::begin() {
  return subworld.begin();
}

Level::iterator Level::end() {
  return subworld.end();
}
// end Level
}
