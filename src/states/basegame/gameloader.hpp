#pragma once

#include "../../renderstates.hpp"
#include "../../types.hpp"
#include "tiledefs.hpp"

#include <string>

namespace kme {
class TileDefLoader {
public:
  static const StringTable<TileDef::CollisionType> collision_table;

  TileDefLoader(std::string filename = "tiledefs.json");

  void load(TileDefs& tiledefs);

private:
  StringTable<TileDef> tile_data;
};
}
