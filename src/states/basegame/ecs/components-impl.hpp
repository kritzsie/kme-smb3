#pragma once

namespace kme {
template<typename T>
T operator *(const Direction& lhs, const T& rhs) {
  return lhs == Direction::RIGHT ? rhs : -rhs;
}

template<typename T>
T operator *(const T& lhs, const Direction& rhs) {
  return rhs * lhs;
}
}
