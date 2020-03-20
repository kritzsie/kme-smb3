#pragma once

#include "../../geometry/vec2.hpp"
#include "../../types.hpp"
#include "tiledefs.hpp"

#include <array>
#include <string>
#include <unordered_map>

namespace std {
  template<>
  struct hash<kme::Vec2<int16_t>> {
    size_t operator()(const kme::Vec2<int16_t>& key) const {
      // size_t should be at least 32 bits.
      static_assert(sizeof(size_t) >= 2 * sizeof(int16_t),
                    "platform unsupported, word size too small");
      return key.x | key.y << 16;
    }
  };
}

namespace kme {
using Chunk = std::array<std::array<TileID, 16>, 16>;
using ChunkMap = std::unordered_map<Vec2<Int16>, Chunk>;

// TODO: create separate class for Chunk type
class Tile {
public:
  static const TileID none;

  Tile(ChunkMap& chunks, Vec2<Int32> pos);

  Tile& operator =(const TileID& rhs);

  bool operator ==(const TileID& rhs) const;
  bool operator !=(const TileID& rhs) const;

  Vec2<Int32> getPos() const;

  Vec2<std::size_t> getLocalPos() const;

  Vec2<Int16> getChunkPos() const;

  const Chunk& getChunk() const;

  operator TileID() const;

  operator bool() const;

  // mutable accessors
  Chunk& getChunk();

private:
  ChunkMap& chunks;

  const Vec2<Int32> pos;
};

class Tilemap {
public:
  class Proxy {
  public:
    Proxy(ChunkMap& chunks, Int32 x);

    Tile operator [](Int32 y);

  private:
    ChunkMap& chunks;

    const Int32 x;
  };

  Proxy operator [](Int32 x);
  Tile operator [](Vec2<Int32> pos);

  // TODO: implement iterator functions

private:
  ChunkMap chunks;
};
}
