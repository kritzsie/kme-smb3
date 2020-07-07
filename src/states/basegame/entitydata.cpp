#include "entitydata.hpp"

#include <sstream>

namespace kme {
const ComponentTypes::Types& EntityData::getDefaults(EntityType type) const {
  return defaults.at(type);
}

void EntityData::registerFlags(EntityType type, UInt32 f) {
  if (flags.find(type) != flags.end()) {
    std::stringstream ss;
    ss << "attempted to redefine flags of entity \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  flags[type] = f;
}

const UInt32& EntityData::getFlags(EntityType type) const {
  return flags.at(type);
}

void EntityData::registerCollisionBox(EntityType type, Box cb) {
  if (collision_boxes.find(type) != collision_boxes.end()) {
    std::stringstream ss;
    ss << "attempted to redefine collision box of entity \"" << type << "\"";
    throw EntityRedefinitionError(ss.str());
  }
  collision_boxes[type] = cb;
}

const Box& EntityData::getCollisionBox(EntityType type) const {
  return collision_boxes.at(type);
}

void EntityData::registerRenderStates(EntityType type, RenderStates rs) {
  registerRenderStates(type, std::make_unique<RenderStates>(rs));
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
