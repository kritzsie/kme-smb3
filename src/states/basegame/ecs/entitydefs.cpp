#include "entitydefs.hpp"

#include "../entity.hpp"
#include "components.hpp"

#include <sstream>

namespace kme {
void EntityDefs::registerHitboxes(EntityType type, Hitboxes states) {
  if (hitboxes.find(type) != hitboxes.end()) {
    std::stringstream ss;
    ss << "attempted to redefine hitboxes of entity \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  hitboxes[type] = states;
}

const EntityDefs::Hitboxes& EntityDefs::getHitboxes(EntityType type) const {
  return hitboxes.at(type);
}

void EntityDefs::registerRenderStates(EntityType type, RenderStates rs) {
  if (render_states.find(type) != render_states.end()) {
    std::stringstream ss;
    ss << "attempted to redefine render states of entity \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  render_states[type] = rs;
}

const RenderStates& EntityDefs::getRenderStates(EntityType type) const {
  return render_states.at(type);
}
}
