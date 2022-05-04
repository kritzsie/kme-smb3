#include "mainmenu.hpp"

#include "../engine.hpp"
#include "../renderer.hpp"
#include "basegame.hpp"

#include <SFML/Window.hpp>

namespace kme {
MainMenu::Factory MainMenu::create() {
  return [](BaseState*, Engine* engine) -> BaseState* {
    return new MainMenu(engine);
  };
}

bool MainMenu::handleInput(sf::Event::EventType type, const sf::Event& event) {
  return not paused;
}

void MainMenu::enter() {
  inputs[Action::UP]       = 0;
  inputs[Action::DOWN]     = 0;
  inputs[Action::LEFT]     = 0;
  inputs[Action::RIGHT]    = 0;
  inputs[Action::SELECT]   = 0;
  inputs[Action::BACK]     = 0;

  keybinds[sf::Keyboard::Key::Left]   = Action::LEFT;
  keybinds[sf::Keyboard::Key::Right]  = Action::RIGHT;
  keybinds[sf::Keyboard::Key::Up]     = Action::UP;
  keybinds[sf::Keyboard::Key::Down]   = Action::DOWN;
  keybinds[sf::Keyboard::Key::Enter]  = Action::SELECT;
  keybinds[sf::Keyboard::Key::Escape] = Action::BACK;

  axisbinds[std::tuple(0, sf::Joystick::Axis::X, Sign::MINUS)] = Action::LEFT;
  axisbinds[std::tuple(0, sf::Joystick::Axis::X, Sign::PLUS)]  = Action::RIGHT;
  axisbinds[std::tuple(0, sf::Joystick::Axis::Y, Sign::MINUS)] = Action::UP;
  axisbinds[std::tuple(0, sf::Joystick::Axis::Y, Sign::PLUS)]  = Action::DOWN;

  axisbinds[std::tuple(0, sf::Joystick::Axis::PovX, Sign::MINUS)] = Action::LEFT;
  axisbinds[std::tuple(0, sf::Joystick::Axis::PovX, Sign::PLUS)]  = Action::RIGHT;
  axisbinds[std::tuple(0, sf::Joystick::Axis::PovY, Sign::MINUS)] = Action::UP;
  axisbinds[std::tuple(0, sf::Joystick::Axis::PovY, Sign::PLUS)]  = Action::DOWN;

  buttonbinds[std::tuple(0, 0)] = Action::SELECT;
  buttonbinds[std::tuple(0, 1)] = Action::BACK;
}

void MainMenu::exit() {}

void MainMenu::pause() {
  paused = true;
}

void MainMenu::resume() {
  paused = false;
}

void MainMenu::update(float delta) {
  if (not paused) {
    engine->pushState(BaseGame::create());
    pause();
  }
}

void MainMenu::draw(float delta) {
  engine->getWindow();
}

MainMenu::MainMenu(Engine* engine) : BaseState(nullptr, engine) {}
}
