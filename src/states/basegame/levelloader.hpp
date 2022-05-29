#pragma once

#include "../../math.hpp"
#include "entity.hpp"
#include "tilemap.hpp"
#include "world.hpp"

#include <string>
#include <unordered_map>

namespace kme {
class LevelLoader {
public:
  struct EntityData {
    std::vector<EntityType> types;
    std::vector<Vec2f> pos;
  };

  struct SubworldData {
    EntityData entities;
    Tilemap tilemap;
    Rect<int> bounds;
    std::string theme;
  };

  LevelLoader(std::size_t world, std::size_t level);

  void load(Level& level);

private:
  std::unordered_map<std::size_t, SubworldData> subworlds;
};
}
