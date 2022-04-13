#pragma once

#include "input.hpp"

namespace kme {
template<typename TBase, typename TWide>
Input<TBase, TWide>::Input() : state(), delta() {}

template<typename TBase, typename TWide>
Input<TBase, TWide>::Input(const Input& input)
: state(input.state), state_prev(input.state_prev), delta(input.delta) {}

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
  state = rhs;
  return *this;
}

template<typename TBase, typename TWide>
void Input<TBase, TWide>::update() {
  delta = state - state_prev;
  state_prev = state;
}
}
