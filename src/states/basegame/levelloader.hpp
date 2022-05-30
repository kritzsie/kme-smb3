#pragma once

#include "../../math.hpp"
#include "entity.hpp"
#include "tilemap.hpp"
#include "world.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace kme {
class LevelLoader {
public:
  struct SubworldData {
    EntityData entities;
    Tilemap tilemap;
    Rect<int> bounds;
    std::string theme;
    std::optional<int> water_height;
  };

  LevelLoader(std::size_t world, std::size_t level);

  void load(Level& level);

private:
  std::unordered_map<std::size_t, SubworldData> subworlds;
};
}
