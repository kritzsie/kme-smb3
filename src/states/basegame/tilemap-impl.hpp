#pragma once

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
