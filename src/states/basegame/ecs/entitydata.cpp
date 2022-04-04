#include "entitydata.hpp"

#include <memory>
#include <sstream>

namespace kme {
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

const std::shared_ptr<RenderStates> EntityData::getRenderStates(EntityType type) const {
  return render_states.at(type);
}
}
