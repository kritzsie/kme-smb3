#include "input.hpp"

namespace kme {
template<typename TBase, typename TWide>
Input<TBase, TWide>::Input() : state(), delta() {}

template<typename TBase, typename TWide>
Input<TBase, TWide>::Input(const Input& input)
: state(input.state), delta(input.delta) {}

template<typename TBase, typename TWide>
TBase Input<TBase, TWide>::getState() const {
  return state;
}

template<typename TBase, typename TWide>
TWide Input<TBase, TWide>::getDelta() const {
  return delta;
}

template<typename TBase, typename TWide>
Input<TBase, TWide>::operator TBase() const {
  return getState();
}

template<typename TBase, typename TWide>
TWide Input<TBase, TWide>::operator ~() const {
  return getDelta();
}

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

template class Input<bool, Int8>;
template class Input<Int8, Int16>;
template class Input<Int16, Int32>;
template class Input<Int32, Int64>;

template class Input<float, double>;
template class Input<double, long double>;
template class Input<long double, long double>;
}
