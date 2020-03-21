#pragma once

#include "../../geometry/vec2.hpp"
#include "../../types.hpp"

#include <string>
#include <vector>

namespace kme {
using EntityID = UInt32;
using EntityType = std::string;

struct EntityComponents {
public:
  std::vector<EntityID> id;
  std::vector<EntityType> type;
  std::vector<Vec2f> pos;
  std::vector<Vec2f> vel;

  void createEntity();

private:
  UInt32 counter;
};

class Entity {
public:
  Entity(EntityComponents& components, UInt32 index);

  const EntityID& getId() const;
  const EntityType& getType() const;
  const Vec2f& getPos() const;
  const Vec2f& getVel() const;

  // mutable accessors
  EntityID& getId();
  EntityType& getType();
  Vec2f& getPos();
  Vec2f& getVel();

private:
  EntityComponents& components;
  UInt32 index;
};
}
