#include "mainmenu.hpp"

#include "../assetmanager.hpp"
#include "../engine.hpp"
#include "../geometry.hpp"
#include "../renderer.hpp"
#include "../types.hpp"
#include "../util.hpp"
#include "basegame.hpp"

#include <SFML/Window.hpp>

#include <string>

#include <cstddef>

namespace kme {
MainMenu::Factory MainMenu::create() {
  return [](BaseState*, Engine* engine) -> BaseState* {
    return new MainMenu(engine);
  };
}

MainMenu::MainMenu(Engine* engine) : BaseState(nullptr, engine) {
  framebuffer = new sf::RenderTexture;
  framebuffer->create(480, 270);

  engine->music->open("title.spc");
}

MainMenu::~MainMenu() {
  delete framebuffer;
}

// TODO: de-duplicate this code in Gameplay::handleInput
bool MainMenu::handleInput(sf::Event::EventType type, const sf::Event& event) {
  if (paused) {
    return false;
  }

  switch (type) {
  case sf::Event::KeyPressed:
  case sf::Event::KeyReleased: {
    auto it = keybinds.find(event.key.code);
    if (it != keybinds.end()) {
      inputs[it->second] = sf::Keyboard::isKeyPressed(event.key.code);
    }
    break;
  }
  case sf::Event::JoystickMoved: {
    const auto& js = event.joystickMove;
    Sign sign = toSign(js.position);
    float pos = std::abs(js.position / 100.f);
    auto it_pos = axisbinds.find(std::tuple(js.joystickId, js.axis, Sign::PLUS));
    if (it_pos != axisbinds.end()) {
      inputs[it_pos->second] = sign == Sign::PLUS ? pos : 0;
    }
    auto it_neg = axisbinds.find(std::tuple(js.joystickId, js.axis, Sign::MINUS));
    if (it_neg != axisbinds.end()) {
      inputs[it_neg->second] = sign == Sign::MINUS ? pos : 0;
    }
  }
  case sf::Event::JoystickButtonPressed:
  case sf::Event::JoystickButtonReleased: {
    const auto& js = event.joystickButton;
    auto it = buttonbinds.find(std::tuple(js.joystickId, js.button));
    if (it != buttonbinds.end()) {
      inputs[it->second] = sf::Joystick::isButtonPressed(js.joystickId, js.button);
    }
    break;
  }
  default:
    break;
  }

  return true;
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
    for (auto& input : inputs) {
      input.second.update();
    }

    const auto& up   = inputs.at(Action::UP);
    const auto& down = inputs.at(Action::DOWN);

    bool select = inputs.at(Action::SELECT) > 0.25f;
    bool back   = inputs.at(Action::BACK) > 0.25f;

    int y = 0;
    y += down > 0.25f and down - ~down <= 0.25f;
    y -= up   > 0.25f and up   - ~up   <= 0.25f;

    if (y) {
      entry = (entry + y) % 2;
      engine->sound->play("mapmove");
    }

    if (select) {
      switch (entry) {
      case 0:
        engine->pushState(BaseGame::create());
        pause();
        break;
      case 1:
        engine->popState();
        break;
      }
    }

    if (back) {
      engine->popState();
    }

    ticktime += delta;
  }
}

void MainMenu::draw(float delta) {
  if (not paused) {
    Window* window = engine->getWindow();

    if (window != nullptr) {
      framebuffer->clear();

      auto fbsize = framebuffer->getSize();
      const auto& background = gfx.getTexture("bonusquestion");
      auto bgsize = background.getSize();

      for (std::size_t y = 0; y <= fbsize.y / bgsize.y; ++y)
      for (std::size_t x = 0; x <= fbsize.x / bgsize.x; ++x) {
        sf::Sprite sprite(background);
        sprite.setPosition(x * bgsize.x, y * bgsize.y);
        framebuffer->draw(sprite);
      }

      drawText(framebuffer, "START GAME", Vec2f(200, 180), TextStyle("smb3_sbfont"));
      drawText(framebuffer, "QUIT", Vec2f(200, 188), TextStyle("smb3_sbfont"));

      drawText(framebuffer, util::highASCII(">"), Vec2f(192, 180 + entry * 8), TextStyle("smb3_sbfont"));

      framebuffer->display();

      window->getFramebuffer()->draw(sf::Sprite(framebuffer->getTexture()));
    }

    rendertime += delta;
  }
}
}
