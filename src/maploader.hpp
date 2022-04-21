#pragma once

#include "states/basegame/tilemap.hpp"
#include "geometry.hpp"

#include <json/value.h>

#include <map>
#include <vector>

namespace kme {
class MapLoader {
public:
  using TileIDs = std::map<std::size_t, TileID>;

  MapLoader(std::string name);

  Rect<int> getBounds() const;
  void loadTiles(Tilemap& tilemap);

private:
  Json::Value root;

  Rect<int> bounds;

  TileIDs tileids;

  void loadTileIDs(std::string path, std::size_t offset);
};
}
