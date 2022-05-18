#pragma once

namespace kme {
template<typename T>
Component<T>::operator const T&() const {
  return value;
}

template<typename T>
Component<T>::operator T&() {
  return const_cast<T&>(static_cast<const Component<T>*>(this)->operator const T&());
}
}
