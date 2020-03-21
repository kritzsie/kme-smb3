#include "entitydata.hpp"

#include <sstream>

namespace kme {
EntityRedefinitionError::EntityRedefinitionError(const char* what)
: EntityRedefinitionError(std::string(what)) {}

EntityRedefinitionError::EntityRedefinitionError(std::string what)
: std::runtime_error(what) {}

void EntityData::registerRenderStates(EntityType type, RenderStates rs) {
  registerRenderStates(type, std::make_unique<RenderStates>(rs));
}

void EntityData::registerRenderStates(EntityType type, std::unique_ptr<RenderStates> rs) {
  registerRenderStates(type, std::shared_ptr<RenderStates>(std::move(rs)));
}

void EntityData::registerRenderStates(EntityType type, std::shared_ptr<RenderStates> rs) {
  if (render_states.find(type) != render_states.end()) {
    std::stringstream ss;
    ss << "attempted to redefine entity with type \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  render_states[type] = rs;
}

std::shared_ptr<RenderStates> EntityData::getRenderStates(EntityType type) {
  return render_states.at(type);
}
}
