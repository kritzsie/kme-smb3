#include "tiledefs.hpp"

#include <sstream>

#include <cmath>

namespace kme {
// begin TileDef
TileDef::TileDef() : collision_type(CollisionType::SOLID) {}

TileDef::CollisionType TileDef::getCollisionType() const {
  return collision_type;
}

void TileDef::setCollisionType(CollisionType new_collision_type) {
  collision_type = new_collision_type;
}

void TileDef::pushFrame(std::string texture, Vec2i origin, float duration) {
  frames.pushFrame(RenderFrame {
    .texture = texture,
    .cliprect = Rect<int>(origin, Vec2i(16, 16)),
    .offset = Vec2f(0.f, 0.f),
    .duration = duration
  });
}

std::size_t TileDef::getFrameCount() const {
  return frames.getFrameCount();
}

// NOTE: this function has potential for optimization
std::size_t TileDef::getFrameOffset(float time) const {
  return frames.getFrameOffset(time);

  /*
  float time_max = 0.f;

  for (auto& frame : frames) {
    time_max += frame.duration;
  }

  float time_mod = std::fmod(time, time_max);
  float accumulator = 0.f;
  std::size_t counter = 0;

  while (time_mod >= accumulator) {
    accumulator += frames[counter++].duration;
  }

  return (counter - 1) % frames.size();
  */
}

const RenderFrame& TileDef::getFrame(std::size_t index) const {
  return frames.getFrame(index);
}
// end TileDef

// begin TileDefs
// ugly
static TileDef getDefaultTileDef() {
  TileDef tiledef;
  tiledef.setCollisionType(TileDef::CollisionType::NONE);
  tiledef.pushFrame("", Vec2i(), 0.f);
  return tiledef;
}

TileDefs::TileDefs() : TileDefs(getDefaultTileDef()) {}

TileDefs::TileDefs(TileDef default_tile) {
  registerTileDef("", std::move(default_tile));
}

void TileDefs::registerTileDef(TileType tile_type, TileDef tiledef) {
  if (tiledefs.find(tile_type) != tiledefs.end()) {
    std::stringstream ss;
    ss << "attempted to redefine tile with id \"" << tile_type << "\"";
    throw TileRedefinitionError(ss.str());
  }
  tiledefs[tile_type] = std::move(tiledef);
}

const TileDef& TileDefs::getTileDef(TileType tile_type) const {
  return tiledefs.at(tile_type);
}

const TileDefs::const_iterator TileDefs::begin() const { return tiledefs.begin(); }
const TileDefs::const_iterator TileDefs::end() const { return tiledefs.end(); }
const TileDefs::const_reverse_iterator TileDefs::rbegin() const { return tiledefs.rbegin(); }
const TileDefs::const_reverse_iterator TileDefs::rend() const { return tiledefs.rend(); }
const TileDefs::const_iterator TileDefs::cbegin() const { return begin(); }
const TileDefs::const_iterator TileDefs::cend() const { return end(); }
const TileDefs::const_reverse_iterator TileDefs::crbegin() const { return rbegin(); }
const TileDefs::const_reverse_iterator TileDefs::crend() const { return rend(); }
TileDefs::iterator TileDefs::begin() { return tiledefs.begin(); }
TileDefs::iterator TileDefs::end() { return tiledefs.end(); }
TileDefs::reverse_iterator TileDefs::rbegin() { return tiledefs.rbegin(); }
TileDefs::reverse_iterator TileDefs::rend() { return tiledefs.rend(); }
// end TileDefs
}
