#include "tilemap.hpp"

#include "../../util/math.hpp"

namespace kme {
// begin Tile
const TileID Tile::none;

Tile::Tile(ChunkMap& chunks, Vec2<Int32> pos) : chunks(chunks), pos(pos) {}

Tile& Tile::operator =(const TileID& rhs) {
  Chunk& chunk = chunks[getChunkPos()];
  Vec2<std::size_t> local_pos = getLocalPos();
  chunk[local_pos.x][local_pos.y] = rhs;
  return *this;
}

bool Tile::operator ==(const TileID& rhs) const {
  return operator TileID() == rhs;
}

bool Tile::operator !=(const TileID& rhs) const {
  return not operator ==(rhs);
}

Vec2<Int32> Tile::getPos() const {
  return pos;
}

Vec2<std::size_t> Tile::getLocalPos() const {
  return Vec2<std::size_t>(util::absmod(pos.x, 16), util::absmod(pos.y, 16));
}

Vec2<Int16> Tile::getChunkPos() const {
  return Vec2<Int16>(util::absdiv(pos.x, 16), util::absdiv(pos.y, 16));
}

const Chunk& Tile::getChunk() const {
  return chunks.at(getChunkPos());
}

Tile::operator TileID() const {
  try {
    Chunk& chunk = chunks.at(getChunkPos());
    Vec2<std::size_t> local_pos = getLocalPos();
    return chunk[local_pos.x][local_pos.y];
  }
  catch (const std::out_of_range&) {
    return none;
  }
}

Tile::operator bool() const {
  return operator TileID().size() > 0;
}

// mutable accessors
Chunk& Tile::getChunk() {
  return const_cast<Chunk&>(static_cast<const Tile*>(this)->getChunk());
}
// end Tile

// begin Tilemap
Tilemap::Proxy::Proxy(ChunkMap& chunks, Int32 x) : chunks(chunks), x(x) {}

Tile Tilemap::Proxy::operator [](Int32 y) {
  return Tile(chunks, Vec2<Int32>(x, y));
}

Tilemap::Proxy Tilemap::operator [](Int32 x) {
  return Proxy(chunks, x);
}

Tile Tilemap::operator [](Vec2<Int32> pos) {
  return operator [](pos.x)[pos.y];
}
// end Tilemap
}
