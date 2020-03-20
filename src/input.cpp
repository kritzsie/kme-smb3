#include "input.hpp"

#include <limits>

namespace kme {
template<typename TBase, typename TWide>
Input<TBase, TWide>::operator TBase() const { return state; }

template<typename TBase, typename TWide>
bool Input<TBase, TWide>::operator ==(const Input& rhs) const {
  return (state == rhs.state);
}

template<typename TBase, typename TWide>
bool Input<TBase, TWide>::operator !=(const Input& rhs) const {
  return (state != rhs.state);
}

template<typename TBase, typename TWide>
bool Input<TBase, TWide>::operator >(const Input& rhs) const {
  return (state > rhs.state);
}

template<typename TBase, typename TWide>
bool Input<TBase, TWide>::operator <(const Input& rhs) const {
  return (state < rhs.state);
}

template<typename TBase, typename TWide>
bool Input<TBase, TWide>::operator >=(const Input& rhs) const {
  return (state >= rhs.state);
}

template<typename TBase, typename TWide>
bool Input<TBase, TWide>::operator <=(const Input& rhs) const {
  return (state <= rhs.state);
}

template<typename TBase, typename TWide>
TWide Input<TBase, TWide>::operator +() const {
  return +state;
}

template<typename TBase, typename TWide>
TWide Input<TBase, TWide>::operator -() const {
  return -state;
}

template<typename TBase, typename TWide>
TWide Input<TBase, TWide>::operator +(const Input& rhs) const {
  return (state + rhs.state);
}

template<typename TBase, typename TWide>
TWide Input<TBase, TWide>::operator -(const Input& rhs) const {
  return (state - rhs.state);
}

template<typename TBase, typename TWide>
Input<TBase, TWide>& Input<TBase, TWide>::operator =(TBase rhs) {
  delta = rhs - state;
  state = rhs;
  return *this;
}

template<typename TBase, typename TWide>
TWide Input<TBase, TWide>::operator ~() const {
  return delta;
}

template class Input<bool, int>;
template class Input<SInt8, int>;
template class Input<short, int>;
template class Input<int, long long>;
template class Input<long, long long>;
template class Input<long long, long long>;

template class Input<float, double>;
template class Input<double, long double>;
template class Input<long double, long double>;
}
