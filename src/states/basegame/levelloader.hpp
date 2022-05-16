#pragma once

#include "../../math.hpp"
#include "tilemap.hpp"
#include "world.hpp"

#include <map>
#include <string>

namespace kme {
class LevelLoader {
public:
  struct SubworldData {
    Tilemap tiles;
    Rect<int> bounds;
    std::string theme;
  };

  LevelLoader(std::size_t world, std::size_t level);

  void load(Level& level);

private:
  std::map<std::size_t, SubworldData> subworld_data;
};
}
