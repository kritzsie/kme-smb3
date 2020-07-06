#pragma once

#include "entitycomponentmanager.hpp"
#include "../../renderstates.hpp"

#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace kme {
class EntityRedefinitionError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class EntityData {
public:
  void registerFlags(EntityType type, UInt32 f);
  const UInt32& getFlags(EntityType type) const;

  void registerCollisionBox(EntityType type, Box cb);
  const Box& getCollisionBox(EntityType type) const;

  void registerRenderStates(EntityType type, RenderStates rs);
  void registerRenderStates(EntityType type, std::unique_ptr<RenderStates> rs);
  void registerRenderStates(EntityType type, std::shared_ptr<RenderStates> rs);
  const std::shared_ptr<RenderStates> getRenderStates(EntityType type) const;

private:
  std::unordered_map<EntityType, UInt32> flags;
  std::unordered_map<EntityType, Box> collision_boxes;
  std::unordered_map<EntityType, std::shared_ptr<RenderStates>> render_states;
};
}
