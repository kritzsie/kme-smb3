#pragma once

#include "states/basegame/tilemap.hpp"
#include "geometry.hpp"

#include <json/value.h>

#include <vector>

namespace kme {
class MapLoader {
public:
  MapLoader(std::string name);

  Rect<int> getBounds();
  void loadTiles(Tilemap& tiles);

private:
  Json::Value root;

  Rect<int> bounds;
};
}
