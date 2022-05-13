#pragma once

#include "../../math.hpp"
#include "../../types.hpp"
#include "tiledefs.hpp"

#include <array>
#include <string>
#include <unordered_map>

namespace kme {
using namespace vec2_aliases;

using Chunk = std::array<std::array<TileID, 16>, 16>;
using ChunkMap = std::unordered_map<Vec2s, Chunk>;

// TODO: create separate class for Chunk type
class Tile {
public:
  static const inline TileID none;

  Tile(ChunkMap& chunks, Vec2i pos);

  Tile& operator =(const TileID& rhs);

  bool operator ==(const TileID& rhs) const;
  bool operator !=(const TileID& rhs) const;

  Vec2i getPos() const;

  Vec2z getLocalPos() const;

  Vec2s getChunkPos() const;

  const Chunk& getChunk() const;
  Chunk& getChunk();

  operator TileID() const;

  operator bool() const;

private:
  ChunkMap& chunks;

  const Vec2<int> pos;
};

class Tilemap {
public:
  class Proxy {
  public:
    Proxy(ChunkMap& chunks, int x);

    Tile operator [](int y);

  private:
    ChunkMap& chunks;

    const int x;
  };

  Proxy operator [](int x);
  Tile operator [](Vec2<int> pos);

  const ChunkMap& getChunks() const;
  ChunkMap& getChunks();

private:
  ChunkMap chunks;
};
}
