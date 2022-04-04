#pragma once

#include "../../../renderstates.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace kme {
using EntityType = std::string;

class EntityRedefinitionError : public std::runtime_error {
public:
  using runtime_error::runtime_error;
};

class EntityData {
public:
  void registerRenderStates(EntityType type, RenderStates rs);
  void registerRenderStates(EntityType type, std::unique_ptr<RenderStates> rs);
  void registerRenderStates(EntityType type, std::shared_ptr<RenderStates> rs);
  const std::shared_ptr<RenderStates> getRenderStates(EntityType type) const;

private:
  std::unordered_map<EntityType, std::shared_ptr<RenderStates>> render_states;
};
}
