#include "entitycomponentmanager.hpp"

#include <algorithm>

namespace kme {
EntityComponentManager::EntityComponentManager() noexcept
: capacity(DEFAULT_CAPACITY) {}

std::size_t EntityComponentManager::getCapacity() const noexcept {
  return capacity;
}

void EntityComponentManager::setCapacity(std::size_t requested) {
  std::size_t actual = std::max(requested, DEFAULT_CAPACITY);
  std::apply([actual](auto&&... args) {
    (args.value.reserve(actual), ...);
  }, components);
  capacity = actual;
}

EntityID EntityComponentManager::createEntity() noexcept {
  entities.insert(slot);
  std::apply([this](auto&&... args) {
    (args.value[slot], ...);
  }, components);
  return slot++;
}

void EntityComponentManager::removeEntity(EntityID entity) {
  std::apply([entity](auto&&... args) {
    (args.value.erase(entity), ...);
  }, components);
  entities.erase(entity);
}

const EntityComponentManager::const_iterator EntityComponentManager::begin() const {
  return entities.begin();
}

const EntityComponentManager::const_iterator EntityComponentManager::end() const {
  return entities.end();
}

const EntityComponentManager::const_reverse_iterator EntityComponentManager::rbegin() const {
  return entities.rbegin();
}

const EntityComponentManager::const_reverse_iterator EntityComponentManager::rend() const {
  return entities.rend();
}

const EntityComponentManager::const_iterator EntityComponentManager::cbegin() const {
  return begin();
}

const EntityComponentManager::const_iterator EntityComponentManager::cend() const {
  return end();
}

const EntityComponentManager::const_reverse_iterator EntityComponentManager::crbegin() const {
  return rbegin();
}

const EntityComponentManager::const_reverse_iterator EntityComponentManager::crend() const {
  return rend();
}

EntityComponentManager::iterator EntityComponentManager::begin() {
  return entities.begin();
}

EntityComponentManager::iterator EntityComponentManager::end() {
  return entities.end();
}

EntityComponentManager::reverse_iterator EntityComponentManager::rbegin() {
  return entities.rbegin();
}

EntityComponentManager::reverse_iterator EntityComponentManager::rend() {
  return entities.rend();
}
}
