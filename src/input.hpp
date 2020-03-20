#pragma once

#include "types.hpp"

#include <cstdint>

namespace kme {
template<typename TBase, typename TWide>
class Input {
protected:
  TBase state;

public:
  TWide delta;

  operator TBase() const;

  bool operator ==(const Input&) const;
  bool operator !=(const Input&) const;

  bool operator >(const Input&) const;
  bool operator <(const Input&) const;
  bool operator >=(const Input&) const;
  bool operator <=(const Input&) const;

  TWide operator +() const;
  TWide operator -() const;

  TWide operator +(const Input&) const;
  TWide operator -(const Input&) const;

  Input& operator =(TBase);

  TWide operator ~() const;
};

using Axis = Input<short, int>;
using Button = Input<bool, int>;
}
