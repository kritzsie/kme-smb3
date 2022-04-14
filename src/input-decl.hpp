#pragma once

#include "types.hpp"
#include "util.hpp"

#include <cstdint>

namespace kme {
template<typename TBase, typename TWide = typename Widen<TBase>::type>
class Input {
private:
  TBase state;
  TBase state_prev;
  TWide delta;

public:
  Input();
  Input(const Input& input);

  Input& operator =(TBase rhs);

  void update();

  TBase getState() const;
  TWide getDelta() const;

  operator TBase() const;
  TWide operator ~() const; // same as getDelta()

  bool operator ==(const Input& rhs) const;
  bool operator !=(const Input& rhs) const;

  bool operator >(const Input& rhs) const;
  bool operator <(const Input& rhs) const;
  bool operator >=(const Input& rhs) const;
  bool operator <=(const Input& rhs) const;

  TWide operator +() const;
  TWide operator -() const;

  TWide operator +(const Input& rhs) const;
  TWide operator -(const Input& rhs) const;
};
}
