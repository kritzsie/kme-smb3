#pragma once

#include "../../../renderstates.hpp"
#include "../powerup.hpp"
#include "components.hpp"

#include <map>
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
  using Hitboxes = std::map<Powerup, std::map<EState, Hitbox>>;

  void registerHitboxes(EntityType type, std::shared_ptr<Hitboxes> states);
  std::shared_ptr<const Hitboxes> getHitboxes(EntityType type) const;

  void registerRenderStates(EntityType type, RenderStates rs);
  void registerRenderStates(EntityType type, std::unique_ptr<RenderStates> rs);
  void registerRenderStates(EntityType type, std::shared_ptr<RenderStates> rs);
  std::shared_ptr<const RenderStates> getRenderStates(EntityType type) const;

private:
  std::unordered_map<EntityType, std::shared_ptr<Hitboxes>> hitboxes;
  std::unordered_map<EntityType, std::shared_ptr<RenderStates>> render_states;
};

// fully-qualified renderstate label that takes powerup states into account
std::string getRenderStateLabel(Powerup powerup, EState state);
}
