#pragma once

#include "../../../renderstates.hpp"
#include "../powerup.hpp"
#include "components.hpp"

#include <map>
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

  void registerHitboxes(EntityType type, Hitboxes states);
  const Hitboxes& getHitboxes(EntityType type) const;

  void registerRenderStates(EntityType type, RenderStates rs);
  const RenderStates& getRenderStates(EntityType type) const;

private:
  std::unordered_map<EntityType, Hitboxes> hitboxes;
  std::unordered_map<EntityType, RenderStates> render_states;
};
}
