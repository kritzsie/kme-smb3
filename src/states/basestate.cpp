#include "basestate.hpp"

namespace kme {
BaseState::BaseState(BaseState* parent, Engine* engine) : parent(parent), engine(engine) {}

bool BaseState::handleInput(sf::Event::EventType, const sf::Event&) {
  return false;
}
}
