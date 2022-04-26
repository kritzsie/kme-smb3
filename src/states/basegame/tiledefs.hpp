#pragma once

#include "../../geometry/rect.hpp"
#include "../../renderstates.hpp"

#include <map>
#include <stdexcept>
#include <vector>

namespace kme {
using TileID = std::string;

class TileRedefinitionError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class TileDef {
public:
  enum class CollisionType {
    NONE, SOLID, PLATFORM, SLOPE
  };

  enum class SlopeType {
    SLOPE_UP, SLOPE_DOWN,
    CEIL_SLOPE_UP, CEIL_SLOPE_DOWN,
    SLOPE_UP_GENTLE_BOTTOM, SLOPE_UP_GENTLE_TOP,
    SLOPE_DOWN_GENTLE_TOP, SLOPE_DOWN_GENTLE_BOTTOM,
    CEIL_SLOPE_UP_GENTLE_BOTTOM, CEIL_SLOPE_UP_GENTLE_TOP,
    CEIL_SLOPE_DOWN_GENTLE_TOP, CEIL_SLOPE_DOWN_GENTLE_BOTTOM
  };

  TileDef();

  CollisionType getCollisionType() const;
  void setCollisionType(CollisionType new_collision_type);

  std::size_t getFrameCount() const;
  std::size_t getFrameOffset(float time) const;

  const RenderFrame& getFrame(std::size_t index) const;
  void pushFrame(std::string texture, Vec2i origin, float duration);

private:
  CollisionType collision_type;
  SlopeType slope_type;

  RenderFrames frames;
};

class TileDefs {
public:
  using Map = std::map<TileID, TileDef>;

  using const_iterator = Map::const_iterator;
  using const_reverse_iterator = Map::const_reverse_iterator;
  using iterator = Map::iterator;
  using reverse_iterator = Map::reverse_iterator;

  TileDefs();
  TileDefs(TileDef default_tile);

  void registerTileDef(TileID tileid, TileDef tiledef);
  const TileDef& getTileDef(TileID tileid) const;

  const const_iterator cbegin() const;
  const const_iterator cend() const;
  const const_iterator begin() const;
  iterator begin();
  const const_iterator end() const;
  iterator end();

  const const_reverse_iterator crbegin() const;
  const const_reverse_iterator crend() const;
  const const_reverse_iterator rbegin() const;
  reverse_iterator rbegin();
  const const_reverse_iterator rend() const;
  reverse_iterator rend();

private:
  Map tiledefs;
};
}
