#include "world.hpp"

namespace kme {
const Tilemap& Subworld::getTiles() const {
  return tiles;
}

Tilemap& Subworld::getTiles() {
  return const_cast<Tilemap&>(static_cast<const Subworld*>(this)->getTiles());
}

Level::Level() : counter(0) {
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
