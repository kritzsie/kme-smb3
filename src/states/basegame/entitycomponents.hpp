#pragma once

#include <tuple>
#include <unordered_map>

namespace kme {
template<typename T>
struct Component {
  using type = T;

  T value;
};

template<typename TID, typename TType, typename... Ts>
struct TypeMap {
  template<typename T>
  using Map = std::unordered_map<TID, T>;

  using Components = std::tuple<Map<TType>, Map<Ts>...>;

  using Defaults = std::unordered_map<typename TType::type, std::tuple<Ts...>>;
};
}
