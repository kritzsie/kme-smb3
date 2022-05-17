#include "tilemap.hpp"

#include "../../util/math.hpp"

namespace kme {
using namespace vec2_aliases;

// begin Tile
Tile::Tile(ChunkMap& chunks, Vec2i pos) : chunks(chunks), pos(pos) {}

Tile& Tile::operator =(const TileID& rhs) {
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

bool Tile::operator ==(const Tile& rhs) const {
  return pos == rhs.pos;
}

bool Tile::operator !=(const Tile& rhs) const {
  return pos != rhs.pos;
}

bool Tile::operator ==(const TileID& rhs) const {
  return getTileID() == rhs;
}

bool Tile::operator !=(const TileID& rhs) const {
  return getTileID() != rhs;
}

TileID Tile::getTileID() const {
  auto iter = chunks.find(getChunkPos());
  if (iter != chunks.end()) {
    Chunk& chunk = iter->second;
    Vec2z local_pos = getLocalPos();
    return chunk[local_pos.x][local_pos.y];
  }
  return none;
}

Vec2i Tile::getPos() const {
  return pos;
}

Vec2z Tile::getLocalPos() const {
  return Vec2z(util::absmod(pos.x, 16), util::absmod(pos.y, 16));
}

Vec2s Tile::getChunkPos() const {
  return Vec2s(util::absdiv(pos.x, 16), util::absdiv(pos.y, 16));
}

const Chunk& Tile::getChunk() const {
  return chunks.at(getChunkPos());
}

Tile::operator TileID() const {
  return getTileID();
}

Tile::operator bool() const {
  return getTileID() != none;
}

// mutable accessors
Chunk& Tile::getChunk() {
  return const_cast<Chunk&>(static_cast<const Tile*>(this)->getChunk());
}
// end Tile

// begin Tilemap
Tilemap::Proxy::Proxy(ChunkMap& chunks, int x) : chunks(chunks), x(x) {}

Tile Tilemap::Proxy::operator [](int y) {
  return Tile(chunks, Vec2i(x, y));
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
