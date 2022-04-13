#pragma once

#include "types.hpp"

#include <cstdint>

namespace kme {
template<typename TBase>
class Input {
private:
  using TWide = typename Widen<TBase>::type;

  TBase state;
  TWide delta;

public:
  Input();
  Input(const Input& input);

  Input& operator =(TBase rhs);

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

using Axis = Input<Int16>;
using Button = Input<bool>;
}
