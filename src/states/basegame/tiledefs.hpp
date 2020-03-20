#pragma once

#include "../../geometry/rect.hpp"
#include "../../renderstates.hpp"

#include <exception>
#include <map>
#include <vector>

namespace kme {
using TileID = std::string;

class TileDef {
public:
  enum class CollisionType {
    NONE, SOLID
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

  RenderFrames frames;
};

class TileRedefinitionError : public std::runtime_error {
public:
  TileRedefinitionError(const char* what);
  TileRedefinitionError(std::string what);
};

class TileDefs {
public:
  using const_iterator = std::map<TileID, TileDef>::const_iterator;
  using const_reverse_iterator = std::map<TileID, TileDef>::const_reverse_iterator;
  using iterator = std::map<TileID, TileDef>::iterator;
  using reverse_iterator = std::map<TileID, TileDef>::reverse_iterator;

  TileDefs();
  TileDefs(TileDef default_tile);

  void registerTileDef(TileID tileid, TileDef tiledef);
  const TileDef& getTileDef(TileID tileid) const;

  const const_iterator begin() const;
  const const_iterator end() const;
  const const_reverse_iterator rbegin() const;
  const const_reverse_iterator rend() const;
  const const_iterator cbegin() const;
  const const_iterator cend() const;
  const const_reverse_iterator crbegin() const;
  const const_reverse_iterator crend() const;

  // mutable accessors
  iterator begin();
  iterator end();
  reverse_iterator rbegin();
  reverse_iterator rend();

private:
  std::map<TileID, TileDef> tiledefs;
};
}
