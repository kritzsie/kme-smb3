#pragma once

#include "../../math.hpp"
#include "tilemap.hpp"
#include "world.hpp"

#include <string>
#include <unordered_map>

namespace kme {
class LevelLoader {
public:
  struct SubworldData {
    Tilemap tilemap;
    Rect<int> bounds;
    std::string theme;
  };

  LevelLoader(std::size_t world, std::size_t level);

  void load(Level& level);

private:
  std::unordered_map<std::size_t, SubworldData> subworld_data;
};
}
