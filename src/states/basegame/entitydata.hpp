#pragma once

#include "entitycomponents.hpp"
#include "../../renderstates.hpp"

#include <unordered_map>

namespace kme {
class EntityData {
public:
  template <typename T>
  using Map = std::unordered_map<EntityType, T>;

  Map<RenderStates> renderstates;
};
}
