#pragma once

#include "input.hpp"
#include "types.hpp"

#include <SFML/Window/Event.hpp>

#include <map>
#include <tuple>

namespace kme {
template<typename TAction>
struct InputHandler {
  using Keys    = std::map<sf::Keyboard::Key, TAction>;
  using Buttons = std::map<std::tuple<UInt, UInt>, TAction>;
  using Axes    = std::map<std::tuple<UInt, sf::Joystick::Axis, Sign>, TAction>;
  using Actions = std::map<TAction, Input<float>>;

  Keys keys;
  Buttons buttons;
  Axes axes;
  Actions actions;

  void update(sf::Event::EventType type, sf::Event event);
};
}
