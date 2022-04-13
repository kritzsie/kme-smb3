#pragma once

#include "input.hpp"

namespace kme {
template<typename TBase>
Input<TBase>::Input() : state(), delta() {}

template<typename TBase>
Input<TBase>::Input(const Input& input)
: state(input.state), delta(input.delta) {}

template<typename TBase>
TBase Input<TBase>::getState() const {
  return state;
}

template<typename TBase>
typename Input<TBase>::TWide Input<TBase>::getDelta() const {
  return delta;
}

template<typename TBase>
Input<TBase>::operator TBase() const {
  return getState();
}

template<typename TBase>
typename Input<TBase>::TWide Input<TBase>::operator ~() const {
  return getDelta();
}

template<typename TBase>
bool Input<TBase>::operator ==(const Input& rhs) const {
  return (state == rhs.state);
}

template<typename TBase>
bool Input<TBase>::operator !=(const Input& rhs) const {
  return (state != rhs.state);
}

template<typename TBase>
bool Input<TBase>::operator >(const Input& rhs) const {
  return (state > rhs.state);
}

template<typename TBase>
bool Input<TBase>::operator <(const Input& rhs) const {
  return (state < rhs.state);
}

template<typename TBase>
bool Input<TBase>::operator >=(const Input& rhs) const {
  return (state >= rhs.state);
}

template<typename TBase>
bool Input<TBase>::operator <=(const Input& rhs) const {
  return (state <= rhs.state);
}

template<typename TBase>
typename Input<TBase>::TWide Input<TBase>::operator +() const {
  return +state;
}

template<typename TBase>
typename Input<TBase>::TWide Input<TBase>::operator -() const {
  return -state;
}

template<typename TBase>
typename Input<TBase>::TWide Input<TBase>::operator +(const Input& rhs) const {
  return (state + rhs.state);
}

template<typename TBase>
typename Input<TBase>::TWide Input<TBase>::operator -(const Input& rhs) const {
  return (state - rhs.state);
}

template<typename TBase>
Input<TBase>& Input<TBase>::operator =(TBase rhs) {
  delta = rhs - state;
  state = rhs;
  return *this;
}
}
