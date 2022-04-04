#pragma once

#include <entt/entt.hpp>

namespace kme {
using Entity = entt::entity;
using EntityRegistry = entt::registry;

template<typename T>
struct Component {
  T value;

  operator const T&() const;
  operator T&();
};
}
