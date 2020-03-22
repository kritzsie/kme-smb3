#pragma once

#include "../../geometry/vec2.hpp"
#include "../../types.hpp"

#include <string>
#include <vector>

namespace kme {
using EntityID = UInt32;
using EntityType = std::string;

struct EntityComponents {
  static const std::size_t DEFAULT_SIZE = 256;

  EntityComponents();

  const std::size_t getSize() const;
  const std::size_t getCapacity() const;

  std::vector<bool> valid;
  std::vector<EntityID> id;
  std::vector<EntityType> type;
  std::vector<Vec2f> pos;
  std::vector<Vec2f> vel;
};
}
