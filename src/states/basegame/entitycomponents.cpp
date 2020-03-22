#include "entitycomponents.hpp"

namespace kme {
EntityComponents::EntityComponents() :
valid(DEFAULT_SIZE),
id(DEFAULT_SIZE),
type(DEFAULT_SIZE),
pos(DEFAULT_SIZE),
vel(DEFAULT_SIZE)
{}

const std::size_t EntityComponents::getSize() const {
  return getCapacity(); // TODO: implement this
}

const std::size_t EntityComponents::getCapacity() const {
  return valid.size();
}
}
