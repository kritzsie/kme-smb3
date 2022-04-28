#pragma once

#include "../../geometry.hpp"
#include "tilemap.hpp"
#include "world.hpp"

#include <json/value.h>

#include <map>
#include <vector>

namespace kme {
class LevelLoader {
public:
  struct SubworldData {
    Json::Value root;
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
