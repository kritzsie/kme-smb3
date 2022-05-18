#include "tilemap.hpp"

#include "../../util/math.hpp"

namespace kme {
using namespace vec2_aliases;

// begin Tile
bool Tile::operator ==(const Tile& rhs) const {
  return layer == rhs.layer and pos == rhs.pos;
}

bool Tile::operator !=(const Tile& rhs) const {
  return layer != rhs.layer or pos != rhs.pos;
}
// end Tile

// begin ChunkTile
ChunkTile::ChunkTile(ChunkMap& chunks, Vec2i pos) : chunks(chunks), pos(pos) {}

ChunkTile& ChunkTile::operator =(const TileID& rhs) {
  Vec2s chunk_pos = getChunkPos();
  auto iter = chunks.find(chunk_pos);
  if (iter != chunks.end()) {
    Chunk& chunk = iter->second;
    Vec2z local_pos = getLocalPos();
    chunk[local_pos.x][local_pos.y] = rhs;
  }
  else if (rhs != none) {
    Chunk& chunk = chunks[chunk_pos];
    Vec2z local_pos = getLocalPos();
    chunk[local_pos.x][local_pos.y] = rhs;
  }
  return *this;
}

bool ChunkTile::operator ==(const ChunkTile& rhs) const {
  return pos == rhs.pos;
}

bool ChunkTile::operator !=(const ChunkTile& rhs) const {
  return pos != rhs.pos;
}

bool ChunkTile::operator ==(const TileID& rhs) const {
  return getTileID() == rhs;
}

bool ChunkTile::operator !=(const TileID& rhs) const {
  return getTileID() != rhs;
}

TileID ChunkTile::getTileID() const {
  auto iter = chunks.find(getChunkPos());
  if (iter != chunks.end()) {
    Chunk& chunk = iter->second;
    Vec2z local_pos = getLocalPos();
    return chunk[local_pos.x][local_pos.y];
  }
  return none;
}

Vec2i ChunkTile::getPos() const {
  return pos;
}

Vec2z ChunkTile::getLocalPos() const {
  return Vec2z(util::absmod(pos.x, 16), util::absmod(pos.y, 16));
}

Vec2s ChunkTile::getChunkPos() const {
  return Vec2s(util::absdiv(pos.x, 16), util::absdiv(pos.y, 16));
}

const Chunk& ChunkTile::getChunk() const {
  return chunks.at(getChunkPos());
}

ChunkTile::operator TileID() const {
  return getTileID();
}

ChunkTile::operator bool() const {
  return getTileID() != none;
}

// mutable accessors
Chunk& ChunkTile::getChunk() {
  return const_cast<Chunk&>(static_cast<const ChunkTile*>(this)->getChunk());
}
// end ChunkTile

// begin Tilemap
Tilemap::Proxy::Proxy(ChunkMap& chunks, int x) : chunks(chunks), x(x) {}

ChunkTile Tilemap::Proxy::operator [](int y) {
  return ChunkTile(chunks, Vec2i(x, y));
}

Tilemap::Proxy Tilemap::operator [](int x) {
  return Proxy(chunks, x);
}

const ChunkMap& Tilemap::getChunks() const {
  return chunks;
}

// mutable accessors
ChunkMap& Tilemap::getChunks() {
  return const_cast<ChunkMap&>(static_cast<const Tilemap*>(this)->getChunks());
}
// end Tilemap
}
