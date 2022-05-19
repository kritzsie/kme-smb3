#pragma once

#include "../../math.hpp"
#include "../../util.hpp"

#include <functional>

template<>
struct std::hash<kme::Tile> {
  std::size_t operator()(const kme::Tile& key) const {
    static constexpr std::hash<decltype(key.pos)> pos_hasher;
    const auto key2 = pos_hasher(key.pos);
    const auto key3 = key.layer;
    return key2 ^ (key3 << 12 | key3 >> 20);
  }
};

namespace kme {
constexpr Vec2s Tilemap::getChunkPos(int x, int y) {
  return Vec2s(util::absdiv(x, 16), util::absdiv(y, 16));
}

constexpr Vec2z Tilemap::getLocalPos(int x, int y) {
  return Vec2z(util::absmod(x, 16), util::absmod(y, 16));
}
}
