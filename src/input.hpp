#pragma once

#include "types.hpp"

#include <cstdint>

namespace kme {
template<typename TBase, typename TWide>
class Input {
private:
  TBase state;
  TWide delta;

public:
  Input();
  Input(const Input& input);

  Input& operator =(TBase);

  TBase getState() const;
  TWide getDelta() const;

  operator TBase() const;
  TWide operator ~() const;

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

using Axis = Input<short, int>;
using Button = Input<bool, signed char>;
}
