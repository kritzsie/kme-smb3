#pragma once

#include "collision.hpp"

#include <functional>
#include <type_traits>

template<>
struct std::hash<kme::WorldCollision> {
  std::size_t operator()(const kme::WorldCollision& key) const {
    using Entity = std::underlying_type_t<decltype(key.entity)>;
    static constexpr std::hash<decltype(key.tile)> tile_hasher;
    const auto key1 = static_cast<Entity>(key.entity);
    const auto key2 = tile_hasher(key.tile);
    return key1 ^ (key2 << 16 | key2 >> 16);
  }
};

template<>
struct std::hash<kme::EntityCollision> {
  std::size_t operator()(const kme::EntityCollision& key) const {
    using Entity = std::underlying_type_t<decltype(key.entity1)>;
    const auto key1 = static_cast<Entity>(key.entity1);
    const auto key2 = static_cast<Entity>(key.entity2);
    return key1 ^ (key2 << 16 | key2 >> 16);
  }
};
