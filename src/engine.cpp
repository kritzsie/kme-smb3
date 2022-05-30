#include "engine.hpp"

#include "math.hpp"
#include "music.hpp"
#include "sound.hpp"
#include "states.hpp"

#include <physfs.h>

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <utility>

#include <cstdlib>

namespace kme {
using namespace vec2_aliases;

std::size_t Engine::instance_count = 0;

TimeInfo::TimeInfo(float rate) : rate(rate), delta(1.f / rate) {}

// Window functions
Window::Window() : Window(1440, 810) {}

Window::Window(std::size_t width, std::size_t height, std::string title)
: sf::RenderWindow(sf::VideoMode(width, height), title) {
  framebuffer.emplace();
  framebuffer->create(480, 270);

  setKeyRepeatEnabled(false);

  sf::VideoMode screen = sf::VideoMode::getDesktopMode();
  setPosition((Vec2i(screen.width, screen.height) - Vec2i(width, height)) / 2);
}

std::optional<sf::RenderTexture>& Window::getFramebuffer() {
  return framebuffer;
}

void Window::drawWindow() {
  clear();
  framebuffer->display();
  draw(sf::Sprite(framebuffer->getTexture()));
  display();
}

void Window::resize(std::size_t width, std::size_t height) {
  Vec2z size = static_cast<sf::Vector2<std::size_t>>(framebuffer->getSize());
  std::size_t scale = std::clamp<std::size_t>(width / size.x, 1, height / size.y);
  sf::View view(Rect<float>(0, 0, width, height));
  view.setCenter(size.x / 2, size.y / 2);
  view.zoom(1.f / scale);
  setView(view);
}

// Engine functions
Engine::Engine(StringList args) : args(args), tickinfo(64.f), renderinfo(60.f) {
  window.emplace(1440, 810, "Super Mario Bros. 3");
  music.emplace();
  sound.emplace();

  if (instance_count == 0) {
    PHYSFS_init(args.at(0).c_str());
  }
  physfsinfo.prefdir = PHYSFS_getPrefDir("klaymore", "smb3");
  physfsinfo.prefdir += "basesmb3/";
  instance_count += 1;

  pushState(Intro::create());
}

Engine::Engine(int argc, char** argv) : Engine(StringList(argv, argv + argc)) {}

Engine::~Engine() {
  if (instance_count > 0) {
    if (instance_count == 1) {
      PHYSFS_deinit();
    }
    instance_count -= 1;
  }
}

std::optional<Window>& Engine::getWindow() {
  return window;
}

bool Engine::isRunning() const {
  return running;
}

TimeInfo Engine::getTickTime() const {
  return tickinfo;
}

TimeInfo Engine::getRenderTime() const {
  return renderinfo;
}

void Engine::pushState(BaseState::Factory factory) {
  events.push_back(StateEvent(StateEventType::PUSH, factory));
}

BaseState* Engine::popState() {
  events.push_back(StateEvent(StateEventType::POP, nullptr));
  return states.back();
}

int Engine::exec() {
  std::cout << "Initializing PhysFS..." << std::flush;
  if (setupPhysFS() == false) {
    std::cout << " Failed!\n";
    std::cerr << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << "\n";
    return EXIT_FAILURE;
  }
  else {
    std::cout << " Done.\n";
  }

  running = true;

  return (main() ? EXIT_SUCCESS : EXIT_FAILURE);
}

bool Engine::main() {
  Duration update_delta = static_cast<Duration>(tickinfo.delta);
  Duration draw_delta = static_cast<Duration>(renderinfo.delta);
  TimePoint time = Clock::now();
  TimePoint time_old = time - update_delta;
  TimePoint update_next = time_old + update_delta;
  TimePoint draw_next = time_old + draw_delta;

  while (running) {
    if (update_next < time) {
      update(tickinfo.delta);
      TimePoint time_new = Clock::now();
      update_next = std::max(update_next, time_new) + update_delta;
    }

    if (draw_next < time) {
      draw(renderinfo.delta);
      TimePoint time_new = Clock::now();
      draw_next = std::max(draw_next, time_new) + draw_delta;
    }

    if (events.size() + states.size() > 0) {
      std::this_thread::sleep_until(std::min(update_next, draw_next));
      time_old = time;
      time = Clock::now();
    }
    else {
      quit();
    }
  }

  return true;
}

// TODO: move event processing to update function
void Engine::update(float delta) {
  if (window) {
    sf::Event event;
    while (window->pollEvent(event)) {
      switch (event.type) {
      case sf::Event::KeyPressed:
      case sf::Event::KeyReleased:
      case sf::Event::JoystickMoved:
      case sf::Event::JoystickButtonPressed:
      case sf::Event::JoystickButtonReleased:
      case sf::Event::MouseMoved:
      case sf::Event::MouseButtonPressed:
      case sf::Event::MouseButtonReleased:
      case sf::Event::MouseWheelScrolled:
        for (auto iter = states.rbegin(); iter != states.rend(); ++iter) {
          if ((*iter)->handleInput(event.type, event)) {
            break;
          }
        }
        break;
      case sf::Event::Resized:
        window->resize(event.size.width, event.size.height);
        break;
      case sf::Event::Closed:
        quit();
        break;
      default:
        break;
      }
    }
  }

  // WARNING: Don't create/delete states from within a BaseState::enter() call!
  // Possible fix: use a stable iterator over the event queue.
  for (StateEvent event : events) {
    BaseState* state;
    switch (event.first) {
    case StateEventType::PUSH:
      state = event.second(states.size() ? states.back() : nullptr, this);
      states.push_back(state);
      state->enter();
      break;
    case StateEventType::POP:
      state = states.back();
      state->exit();
      states.pop_back();
      delete state;
      break;
    }
  }

  events.clear();

  for (BaseState* state : states) {
    state->update(delta);
  }
}

void Engine::draw(float delta) {
  if (window) {
    for (BaseState* state : states) {
      state->draw(delta);
    }

    window->drawWindow();
  }
}

void Engine::quit() {
  for (auto iter = states.rbegin(); iter != states.rend(); ++iter) {
    BaseState* state = *iter;
    state->exit();
  }

  for (auto iter = states.rbegin(); iter != states.rend(); ++iter) {
    delete *iter;
    *iter = nullptr;
  }

  states.clear();

  if (window) {
    window->close();
  }

  running = false;
}

bool Engine::setupPhysFS() {
  PHYSFS_permitSymbolicLinks(true);
  if (PHYSFS_mount(physfsinfo.prefdir.c_str(), "/", false) == 0) {
    std::cerr << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << "\n";
    return false;
  }
  if (PHYSFS_setWriteDir(physfsinfo.prefdir.c_str()) == 0) {
    std::cerr << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()) << "\n";
    return false;
  }
  if (PHYSFS_exists(physfsinfo.prefdir.c_str()) == 0) {
    if (PHYSFS_mkdir("/") == 0) {
      return false;
    }
  }
  return true;
}
}
