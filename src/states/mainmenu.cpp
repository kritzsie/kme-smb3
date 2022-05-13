#include "mainmenu.hpp"

#include "../assetmanager.hpp"
#include "../engine.hpp"
#include "../math.hpp"
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

bool MainMenu::handleInput(sf::Event::EventType type, const sf::Event& event) {
  if (not paused) {
    inputs.update(type, event);
  }

  return not paused;
}

void MainMenu::enter() {
  inputs.actions[Action::UP]     = 0;
  inputs.actions[Action::DOWN]   = 0;
  inputs.actions[Action::LEFT]   = 0;
  inputs.actions[Action::RIGHT]  = 0;
  inputs.actions[Action::SELECT] = 0;
  inputs.actions[Action::BACK]   = 0;

  inputs.keys[sf::Keyboard::Key::Left]   = Action::LEFT;
  inputs.keys[sf::Keyboard::Key::Right]  = Action::RIGHT;
  inputs.keys[sf::Keyboard::Key::Up]     = Action::UP;
  inputs.keys[sf::Keyboard::Key::Down]   = Action::DOWN;
  inputs.keys[sf::Keyboard::Key::Enter]  = Action::SELECT;
  inputs.keys[sf::Keyboard::Key::Escape] = Action::BACK;

  inputs.axes[std::tuple(0, sf::Joystick::Axis::X, Sign::MINUS)] = Action::LEFT;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::X, Sign::PLUS)]  = Action::RIGHT;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::Y, Sign::MINUS)] = Action::UP;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::Y, Sign::PLUS)]  = Action::DOWN;

  inputs.axes[std::tuple(0, sf::Joystick::Axis::PovX, Sign::MINUS)] = Action::LEFT;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::PovX, Sign::PLUS)]  = Action::RIGHT;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::PovY, Sign::MINUS)] = Action::UP;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::PovY, Sign::PLUS)]  = Action::DOWN;

  inputs.buttons[std::tuple(0, 0)] = Action::SELECT;
  inputs.buttons[std::tuple(0, 1)] = Action::BACK;
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
    for (auto& action : inputs.actions) {
      action.second.update();
    }

    const auto& up   = inputs.actions.at(Action::UP);
    const auto& down = inputs.actions.at(Action::DOWN);

    bool select = inputs.actions.at(Action::SELECT) > 0.25f;
    bool back   = inputs.actions.at(Action::BACK) > 0.25f;

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
        engine->sound->play("select");
        pause();
        break;
      case 1:
        engine->popState();
        engine->sound->play("select");
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
      const auto& background = gfx.getTexture("bonusquestion");
      auto bgsize = static_cast<sf::Vector2f>(background.getSize());
      auto fbsize = static_cast<sf::Vector2f>(framebuffer->getSize());

      for (std::size_t y = 0; y < fbsize.y / bgsize.y; ++y)
      for (std::size_t x = 0; x < fbsize.x / bgsize.x; ++x) {
        sf::Sprite sprite(background);
        sprite.setPosition(x * bgsize.x, -(y * bgsize.y + bgsize.y) + fbsize.y);
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
