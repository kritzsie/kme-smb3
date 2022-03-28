#pragma once

#include <entt/entt.hpp>

namespace kme {
using EntityType = entt::entity;

template<typename T>
struct Component {
  T value;

  operator const T&() const;
  operator T&();
};
}
