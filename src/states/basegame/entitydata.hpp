#pragma once

#include "entitycomponentmanager.hpp"
#include "../../renderstates.hpp"

#include <exception>
#include <memory>
#include <unordered_map>

namespace kme {
class EntityRedefinitionError : public std::runtime_error {
public:
  EntityRedefinitionError(const char* what);
  EntityRedefinitionError(std::string what);
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
