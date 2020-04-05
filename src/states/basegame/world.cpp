#include "world.hpp"

namespace kme {
// begin Subworld
Subworld::Subworld(const EntityData& entity_data) : entity_data(entity_data) {}

Entity Subworld::spawnEntity(EntityType type, Vec2f pos) {
  Entity entity = getEntity(entities_next.createEntity());
  entity.set<Type>(type);
  entity.set<Position>(pos);
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

void Subworld::update(float delta) {
  // apply gravity
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    entities_next.get<Velocity>(*iter).y -= 29.625f * delta;
  }

  // move
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    entities_next.get<Position>(*iter) += entities.get<Velocity>(*iter) * delta;
  }

  entities = entities_next;
}
// end Subworld

// begin Level
Level::Level(const EntityData& entity_data) : entity_data(entity_data) {
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

    subworld.emplace(index_hint, Subworld(entity_data));
    return index_hint;
  }

  while (subworld.find(counter) != subworld.end()) {
    ++counter;
  }

  subworld.emplace(counter, Subworld(entity_data));
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
