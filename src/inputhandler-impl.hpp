#pragma once

#include "inputhandler.hpp"

namespace kme {
template<typename T>
void InputHandler<T>::update(sf::Event::EventType type, sf::Event event) {
  switch (type) {
  case sf::Event::KeyPressed:
  case sf::Event::KeyReleased: {
    auto it = keys.find(event.key.code);
    if (it != keys.end()) {
      actions[it->second] = sf::Keyboard::isKeyPressed(event.key.code);
    }
    break;
  }
  case sf::Event::JoystickMoved: {
    const auto& js = event.joystickMove;
    Sign sign = toSign(js.position);
    float pos = std::abs(js.position / 100.f);
    auto it_pos = axes.find(std::tuple(js.joystickId, js.axis, Sign::PLUS));
    if (it_pos != axes.end()) {
      actions[it_pos->second] = sign == Sign::PLUS ? pos : 0;
    }
    auto it_neg = axes.find(std::tuple(js.joystickId, js.axis, Sign::MINUS));
    if (it_neg != axes.end()) {
      actions[it_neg->second] = sign == Sign::MINUS ? pos : 0;
    }
  }
  case sf::Event::JoystickButtonPressed:
  case sf::Event::JoystickButtonReleased: {
    const auto& js = event.joystickButton;
    auto it = buttons.find(std::tuple(js.joystickId, js.button));
    if (it != buttons.end()) {
      actions[it->second] = sf::Joystick::isButtonPressed(js.joystickId, js.button);
    }
    break;
  }
  default:
    break;
  }
}
}
