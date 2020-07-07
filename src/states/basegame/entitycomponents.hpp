#pragma once

#include "../../geometry/vec2.hpp"
#include "../../types.hpp"

#include <string>
#include <tuple>
#include <unordered_map>

namespace kme {
template<typename T>
struct Component {
  using ComponentType = T;

  ComponentType value;
};

template<typename TIndex, typename TKey, typename... Ts>
struct ComponentMap {
  template<typename T>
  using Map = std::unordered_map<TIndex, T>;

  using Types = std::tuple<Ts...>;

  using Components = std::tuple<Map<TKey>, Map<Ts>...>;
  using Defaults = std::unordered_map<typename TKey::ComponentType, Types>;
};

using EntityID = std::size_t;
using EntityType = std::string;

struct Box {
  float radius;
  float height;
};

struct Type : Component<EntityType> {};
struct Parent : Component<EntityID> {};
struct Flags : Component<UInt32> {
  enum : UInt32 {
    NONE      = 0,
    GRAVITY   = 1 << 0,
    LANDED    = 1 << 1
  };
};
struct Position : Component<Vec2f> {};
struct Velocity : Component<Vec2f> {};
struct CollisionBox : Component<Box> {};

using ComponentTypes = ComponentMap<EntityID,
  Type,
  Parent,
  Flags,
  Position,
  Velocity,
  CollisionBox
>;
}
