#pragma once

#include "../../geometry/vec2.hpp"
#include "../../types.hpp"

#include <string>
#include <vector>

namespace kme {
using EntityType = std::string;

struct Entities {
public:
  std::vector<UInt32> id;
  std::vector<EntityType> type;
  std::vector<Vec2f> pos;
  std::vector<Vec2f> vel;

private:
  UInt32 counter;
};

class Entity {
public:
  Entity(Entities& entities, UInt32 index);

  const UInt32& getId() const;
  const EntityType& getType() const;
  const Vec2f& getPos() const;
  const Vec2f& getVel() const;

  // mutable accessors
  UInt32& getId();
  EntityType& getType();
  Vec2f& getPos();
  Vec2f& getVel();

private:
  Entities& entities;
  UInt32 index;
};
}
