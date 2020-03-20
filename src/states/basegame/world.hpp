#pragma once

#include "entities.hpp"
#include "tilemap.hpp"

#include <cstdlib>
#include <unordered_map>

namespace kme {
class Subworld {
public:
  const Tilemap& getTiles() const;

  // mutable accessors
  Tilemap& getTiles();

private:
  Entities entities;
  Tilemap tiles;
};

class Level {
public:
  using SubworldTable = std::unordered_map<UInt32, Subworld>;
  using const_iterator = SubworldTable::const_iterator;
  using iterator = SubworldTable::iterator;

  Level();

  UInt32 createSubworld();
  UInt32 createSubworld(UInt32 index_hint);
  bool subworldExists(UInt32 index);
  bool deleteSubworld(UInt32 index);

  const Subworld& getSubworld(UInt32 index) const;

  const const_iterator begin() const;
  const const_iterator end() const;
  const const_iterator cbegin() const;
  const const_iterator cend() const;

  // mutable accessors
  Subworld& getSubworld(UInt32 index);

  iterator begin();
  iterator end();

private:
  UInt32 counter;

  SubworldTable subworld_original;
  SubworldTable subworld;
};
}
