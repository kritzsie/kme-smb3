#pragma once

#include "../../geometry/vec2.hpp"
#include "../../types.hpp"
#include "tiledefs.hpp"

#include <array>
#include <string>
#include <unordered_map>

namespace std {
  template<>
  struct hash<kme::Vec2<short>> {
    size_t operator()(const kme::Vec2<short>& key) const {
      // size_t should be at least 32 bits.
      static_assert(sizeof(size_t) >= 2 * sizeof(short), "platform unsupported, word size too small");
      return key.x | key.y << 16;
    }
  };
}

namespace kme {
using Chunk = std::array<std::array<TileID, 16>, 16>;
using ChunkMap = std::unordered_map<Vec2<short>, Chunk>;

// TODO: create separate class for Chunk type
class Tile {
public:
  static const inline TileID none;

  Tile(ChunkMap& chunks, Vec2<int> pos);

  Tile& operator =(const TileID& rhs);

  bool operator ==(const TileID& rhs) const;
  bool operator !=(const TileID& rhs) const;

  Vec2<int> getPos() const;

  Vec2<std::size_t> getLocalPos() const;

  Vec2<short> getChunkPos() const;

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

private:
  ChunkMap chunks;
};
}
