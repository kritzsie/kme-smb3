#include "world.hpp"

namespace kme {
Entity::Entity(EntityComponents& entity_components,
               EntityComponents& entity_components_next,
               std::size_t index) :
entity_components(entity_components),
entity_components_next(entity_components_next),
index(index) {}

const EntityID& Entity::getID() const {
  return entity_components.id.at(index);
}

const EntityType& Entity::getType() const {
  return entity_components.type.at(index);
}

const Vec2f& Entity::getPos() const {
  return entity_components.pos.at(index);
}

void Entity::setPos(Vec2f pos) {
  entity_components_next.pos.at(index) = pos;
}

const Vec2f& Entity::getVel() const {
  return entity_components.vel.at(index);
}

void Entity::setVel(Vec2f vel) {
  entity_components_next.vel.at(index) = vel;
}

Entity Subworld::spawnEntity(EntityType type) {
  std::size_t index;
  for (index = 0; index < entity_components_next.getCapacity(); ++index) {
    std::vector<bool>::reference valid = entity_components_next.valid.at(index);
    if (not valid) {
      valid = true;
      entity_components_next.id.at(index) = spawn_counter++;
      entity_components_next.type.at(index) = type;
      return Entity(entity_components, entity_components_next, index);
    }
  }
  expandEntityComponents();
  entity_components_next.valid.at(index) = true;
  entity_components_next.id.at(index) = spawn_counter++;
  entity_components_next.type.at(index) = type;
  return Entity(entity_components, entity_components_next, index);
}

const EntityComponents& Subworld::getEntityComponents() const {
  return entity_components;
}

const Tilemap& Subworld::getTiles() const {
  return tiles;
}

Tilemap& Subworld::getTiles() {
  return const_cast<Tilemap&>(static_cast<const Subworld*>(this)->getTiles());
}

void Subworld::update(float delta_time) {
  for (std::size_t i = 0; i < entity_components.getSize(); ++i) {
    if (entity_components.valid.at(i)) {
      entity_components_next.pos.at(i) += entity_components.vel.at(i) * delta_time;
    }
  }

  entity_components = entity_components_next;
}

void Subworld::expandEntityComponents() {
  std::size_t size = entity_components_next.getCapacity();
  std::size_t new_size = 2 * size; // mimic most vector growth implementations
  try {
    entity_components.valid.resize(new_size);
    entity_components.id.resize(new_size);
    entity_components.type.resize(new_size);
    entity_components.pos.resize(new_size);
    entity_components.vel.resize(new_size);
  }
  catch (const std::bad_alloc& ex) {
    entity_components.valid.resize(size);
    entity_components.id.resize(size);
    entity_components.type.resize(size);
    entity_components.pos.resize(size);
    entity_components.vel.resize(size);
    throw ex;
  }
}

// TODO: implement this for garbage collection
//void Subworld::shrinkEntityComponents() {}

Level::Level() {
  createSubworld();
}

UInt32 Level::createSubworld() {
  return createSubworld(counter++);
}

// TODO: de-duplicate this
UInt32 Level::createSubworld(UInt32 index_hint) {
  if (subworld.find(index_hint) == subworld.end()) {
    if (counter == index_hint) {
      ++counter;
    }

    subworld.emplace(index_hint, Subworld());
    return index_hint;
  }

  while (subworld.find(counter) != subworld.end()) {
    ++counter;
  }

  subworld.emplace(counter, Subworld());
  return counter;
}

bool Level::subworldExists(UInt32 index) {
  return index == 0 or subworld.find(index) != subworld.end();
}

bool Level::deleteSubworld(UInt32 index) {
  if (subworldExists(index)) {
    subworld.erase(index);
    return true;
  }

  return false;
}

const Subworld& Level::getSubworld(UInt32 index) const {
  return subworld.at(index);
}

Subworld& Level::getSubworld(UInt32 index) {
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
}
