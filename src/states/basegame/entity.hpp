#pragma once

#include "../../math.hpp"

#include <entt/entt.hpp>

#include <string>

namespace kme {
using namespace vec2_aliases;

using Entity = entt::entity;
using EntityType = std::string;

struct EntityData {
  std::vector<EntityType> types;
  std::vector<Vec2f> pos;
};
}
