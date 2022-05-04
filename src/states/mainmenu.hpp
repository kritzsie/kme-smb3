#pragma once

#include "../input.hpp"
#include "../types.hpp"
#include "basestate.hpp"

#include <SFML/Window.hpp>

#include <map>
#include <tuple>

namespace kme {
class MainMenu final : public BaseState {
public:
  enum class Action {
    UP, LEFT, DOWN, RIGHT,
    SELECT, BACK
  };

  using KeyMap = std::map<sf::Keyboard::Key, Action>;
  using ButtonMap = std::map<std::tuple<UInt, UInt>, Action>;
  using AxisMap = std::map<std::tuple<UInt, sf::Joystick::Axis, Sign>, Action>;
  using InputMap = std::map<Action, Input<float>>;

public:
  static Factory create();

private:
  MainMenu(Engine* engine);

public:
  ~MainMenu() final = default;

  bool handleInput(sf::Event::EventType type, const sf::Event& event) final;

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update(float delta) final;
  void draw(float delta) final;

private:
  KeyMap keybinds;
  ButtonMap buttonbinds;
  AxisMap axisbinds;
  InputMap inputs;

  bool paused = false;
};
}
