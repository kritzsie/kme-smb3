#include "tilemap.hpp"

#include "../../math.hpp"

namespace kme {
using namespace vec2_aliases;

// begin Tile
Tile::Tile(int layer, int x, int y) : layer(layer), pos(x, y) {}

bool Tile::operator ==(const Tile& rhs) const {
  return layer == rhs.layer and pos == rhs.pos;
}

bool Tile::operator !=(const Tile& rhs) const {
  return layer != rhs.layer or pos != rhs.pos;
}
// end Tile

// begin Tilemap
const Tilemap::Layers& Tilemap::getLayers() const {
  return layers;
}

const Tilemap::Chunks& Tilemap::getChunks(int layer) const {
  return layers.at(layer);
}

void Tilemap::setChunks(int layer, const Chunks& chunks) {
  layers[layer] = chunks;
}

const Tilemap::Chunk& Tilemap::getChunkAt(int layer, int x, int y) const {
  Vec2s chunk_pos = getChunkPos(x, y);
  return getChunks(layer).at(chunk_pos);
}

const Tilemap::Chunk& Tilemap::getChunkAt(Tile tile) const {
  return getChunkAt(tile.layer, tile.pos.x, tile.pos.y);
}

TileID Tilemap::getTile(int layer, int x, int y) const {
  Vec2s chunk_pos = getChunkPos(x, y);
  Vec2z local_pos = getLocalPos(x, y);
  const auto& chunks = getChunks(layer);
  if (chunks.find(chunk_pos) != chunks.end()) {
    return getChunkAt(layer, x, y)[local_pos.y][local_pos.x];
  }
  return notile;
}

TileID Tilemap::getTile(Tile tile) const {
  return getTile(tile.layer, tile.pos.x, tile.pos.y);
}

void Tilemap::setTile(int layer, int x, int y, TileID tileid) {
  Vec2z local_pos = getLocalPos(x, y);
  layers[layer][getChunkPos(x, y)][local_pos.y][local_pos.x] = tileid;
}

void Tilemap::setTile(Tile tile, TileID tileid) {
  setTile(tile.layer, tile.pos.x, tile.pos.y, tileid);
}

// mutable accessors
Tilemap::Layers& Tilemap::getLayers() {
  return const_cast<Layers&>(static_cast<const Tilemap*>(this)->getLayers());
}

Tilemap::Chunks& Tilemap::getChunks(int layer) {
  return const_cast<Chunks&>(static_cast<const Tilemap*>(this)->getChunks(layer));
}

Tilemap::Chunk& Tilemap::getChunkAt(int layer, int x, int y) {
  return const_cast<Chunk&>(static_cast<const Tilemap*>(this)->getChunkAt(layer, x, y));
}

Tilemap::Chunk& Tilemap::getChunkAt(Tile tile) {
  return const_cast<Chunk&>(static_cast<const Tilemap*>(this)->getChunkAt(tile));
}
// end Tilemap
}
