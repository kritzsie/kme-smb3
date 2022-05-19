#pragma once

#include "../../math.hpp"
#include "../../types.hpp"
#include "tiledefs.hpp"

#include <array>
#include <map>
#include <string>
#include <unordered_map>

namespace kme {
using namespace vec2_aliases;

struct Tile {
  int layer;
  Vec2i pos;

  Tile(int layer, int x, int y);

  bool operator ==(const Tile& rhs) const;
  bool operator !=(const Tile& rhs) const;
};

class Tilemap {
public:
  using Chunk = std::array<std::array<TileID, 16>, 16>;
  using Chunks = std::unordered_map<Vec2s, Chunk>;
  using Layers = std::map<int, Chunks>;

  inline static const TileID notile;

  static constexpr Vec2s getChunkPos(int x, int y);
  static constexpr Vec2z getLocalPos(int x, int y);

  const Layers& getLayers() const;
  Layers& getLayers();

  const Chunks& getChunks(int layer) const;
  Chunks& getChunks(int layer);
  void setChunks(int layer, const Chunks& chunks);

  const Chunk& getChunkAt(Tile tile) const;
  const Chunk& getChunkAt(int layer, int x, int y) const;
  Chunk& getChunkAt(Tile tile);
  Chunk& getChunkAt(int layer, int x, int y);

  TileID getTile(Tile tile) const;
  TileID getTile(int layer, int x, int y) const;
  void setTile(int layer, int x, int y, TileID tileid);
  void setTile(Tile tile, TileID tileid);

private:
  Layers layers;
};
}
