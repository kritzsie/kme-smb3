#pragma once

#include "states/basegame/tilemap.hpp"
#include "geometry.hpp"

#include <json/value.h>

#include <map>
#include <vector>

namespace kme {
class MapLoader {
public:
  MapLoader(std::string name);

  Rect<int> getBounds();
  void loadTiles(Tilemap& tilemap, const std::map<UInt, TileID>& tiles);

private:
  Json::Value root;

  Rect<int> bounds;
};
}
