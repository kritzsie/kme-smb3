#include "entitydata.hpp"

#include <sstream>

namespace kme {
void EntityData::registerHitboxes(EntityType type, Hitboxes states) {
  if (hitboxes.find(type) != hitboxes.end()) {
    std::stringstream ss;
    ss << "attempted to redefine hitboxes of entity \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  hitboxes[type] = states;
}

const EntityData::Hitboxes& EntityData::getHitboxes(EntityType type) const {
  return hitboxes.at(type);
}

void EntityData::registerRenderStates(EntityType type, RenderStates rs) {
  if (render_states.find(type) != render_states.end()) {
    std::stringstream ss;
    ss << "attempted to redefine render states of entity \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  render_states[type] = rs;
}

const RenderStates& EntityData::getRenderStates(EntityType type) const {
  return render_states.at(type);
}
}
