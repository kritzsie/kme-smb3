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
  using Chunk = std::array<std::array<TileType, 16>, 16>;
  using Chunks = std::unordered_map<Vec2s, Chunk>;
  using Layers = std::map<int, Chunks>;

  inline static const TileType notile;

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

  TileType getTile(Tile tile) const;
  TileType getTile(int layer, int x, int y) const;
  void setTile(int layer, int x, int y, TileType tile_type);
  void setTile(Tile tile, TileType tile_type);

private:
  Layers layers;
};
}
