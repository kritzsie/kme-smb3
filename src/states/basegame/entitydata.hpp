#pragma once

#include "entitycomponents.hpp"
#include "../../renderstates.hpp"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace kme {
class EntityRedefinitionError : public std::runtime_error {
public:
  using runtime_error::runtime_error;
};

class EntityData {
public:
  template<typename T>
  void setDefault(EntityType type, const typename T::ComponentType& value);

  template<typename T>
  const typename T::ComponentType& getDefault(EntityType type) const;

  const ComponentTypes::Types& getDefaults(EntityType type) const;

  void registerRenderStates(EntityType type, RenderStates rs);
  void registerRenderStates(EntityType type, std::unique_ptr<RenderStates> rs);
  void registerRenderStates(EntityType type, std::shared_ptr<RenderStates> rs);
  const std::shared_ptr<RenderStates> getRenderStates(EntityType type) const;

private:
  ComponentTypes::Defaults defaults;

  std::unordered_map<EntityType, UInt32> flags;
  std::unordered_map<EntityType, Box> collision_boxes;
  std::unordered_map<EntityType, std::shared_ptr<RenderStates>> render_states;
};

template<typename T>
void EntityData::setDefault(EntityType type, const typename T::ComponentType& value) {
  std::get<T>(defaults[type]).value = value;
}

template<typename T>
const typename T::ComponentType& EntityData::getDefault(EntityType type) const {
  return std::get<T>(defaults.at(type)).value;
}
}
