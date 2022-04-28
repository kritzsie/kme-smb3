#include "tilemap.hpp"

#include "../../util/math.hpp"

namespace kme {
// begin Tile
Tile::Tile(ChunkMap& chunks, Vec2<int> pos) : chunks(chunks), pos(pos) {}

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

Vec2<int> Tile::getPos() const {
  return pos;
}

Vec2<std::size_t> Tile::getLocalPos() const {
  return Vec2<std::size_t>(util::absmod(pos.x, 16), util::absmod(pos.y, 16));
}

Vec2<short> Tile::getChunkPos() const {
  return Vec2<short>(util::absdiv(pos.x, 16), util::absdiv(pos.y, 16));
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
Tilemap::Proxy::Proxy(ChunkMap& chunks, int x) : chunks(chunks), x(x) {}

Tile Tilemap::Proxy::operator [](int y) {
  return Tile(chunks, Vec2<int>(x, y));
}

Tilemap::Proxy Tilemap::operator [](int x) {
  return Proxy(chunks, x);
}

Tile Tilemap::operator [](Vec2<int> pos) {
  return operator [](pos.x)[pos.y];
}

const ChunkMap& Tilemap::getChunks() const {
  return chunks;
}

ChunkMap& Tilemap::getChunks() {
  return const_cast<ChunkMap&>(static_cast<const Tilemap*>(this)->getChunks());
}
// end Tilemap
}
