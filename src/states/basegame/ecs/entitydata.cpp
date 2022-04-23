#include "entitydata.hpp"

#include <memory>
#include <sstream>

namespace kme {
void EntityData::registerHitboxes(EntityType type, std::shared_ptr<Hitboxes> states) {
  if (hitboxes.find(type) != hitboxes.end()) {
    std::stringstream ss;
    ss << "attempted to redefine hitboxes of entity \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  hitboxes[type] = states;
}

std::shared_ptr<const EntityData::Hitboxes> EntityData::getHitboxes(EntityType type) const {
  return hitboxes.at(type);
}

void EntityData::registerRenderStates(EntityType type, RenderStates rs) {
  registerRenderStates(type, std::make_shared<RenderStates>(rs));
}

void EntityData::registerRenderStates(EntityType type, std::unique_ptr<RenderStates> rs) {
  registerRenderStates(type, std::shared_ptr<RenderStates>(std::move(rs)));
}

void EntityData::registerRenderStates(EntityType type, std::shared_ptr<RenderStates> rs) {
  if (render_states.find(type) != render_states.end()) {
    std::stringstream ss;
    ss << "attempted to redefine render states of entity \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  render_states[type] = rs;
}

std::shared_ptr<const RenderStates> EntityData::getRenderStates(EntityType type) const {
  return render_states.at(type);
}
}
