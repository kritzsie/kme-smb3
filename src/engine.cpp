#include "engine.hpp"

#include "states.hpp"

#include <physfs.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include <cstdlib>

namespace kme {
std::size_t Engine::instance_count = 0;

TimeInfo::TimeInfo(float rate) : rate(rate), delta(1.f / rate) {}

// Window functions
Window::Window() : Window(1440, 810) {}

Window::Window(UInt32 width, UInt32 height) : Window(width, height, "Klaymore Engine") {}

Window::Window(UInt32 width, UInt32 height, const char* title)
: sf::RenderWindow(sf::VideoMode(width, height), title) {
  framebuffer = new sf::RenderTexture();
  framebuffer->create(480, 270);

  setKeyRepeatEnabled(false);

  sf::VideoMode videomode = sf::VideoMode::getDesktopMode();
  setPosition(sf::Vector2i(
    (videomode.width - width) / 2,
    (videomode.height - height) / 2
  ));
}

Window::~Window() {
  if (framebuffer != nullptr) delete framebuffer;
}

sf::RenderTexture* Window::getFramebuffer() {
  return framebuffer;
}

void Window::drawWindow() {
  clear();

  framebuffer->display();

  sf::Sprite sprite(framebuffer->getTexture());
  draw(sprite);

  display();
}

void Window::resize(UInt32 width, UInt32 height) {
  sf::Vector2u fbsize = framebuffer->getSize();
  UInt32 scale = std::max(1u, std::min(width / fbsize.x, height / fbsize.y));

  sf::View view(sf::FloatRect(0, 0, width, height));
  view.setCenter(fbsize.x / 2, fbsize.y / 2);
  view.zoom(1.0f / scale);
  setView(view);
}

// Engine functions
Engine::Engine(std::vector<std::string>&& args) : args(args), ticktime(64.f), rendertime(60.f) {
  window = new Window(1440, 810, "Super Mario Bros. 3");

  if (instance_count == 0) {
    PHYSFS_init(args.at(0).c_str());
  }
  physfsinfo.prefdir = PHYSFS_getPrefDir("klaymore", "smb3");
  physfsinfo.prefdir += "basesmb3/";
  instance_count += 1;

  pushState(Intro::create());
}

Engine::Engine(int argc, char** argv) : Engine(std::vector<std::string>(argv, argc + argv)) {}

Engine::~Engine() {
  if (window != nullptr) delete window;

  if (instance_count > 0) {
    if (instance_count == 1) {
      PHYSFS_deinit();
    }
    instance_count -= 1;
  }
}

Window* Engine::getWindow() {
  return window;
}

bool Engine::isRunning() const {
  return running;
}

TimeInfo Engine::getTickTime() const {
  return ticktime;
}

TimeInfo Engine::getRenderTime() const {
  return rendertime;
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
  Duration tickdelta = static_cast<Duration>(ticktime.delta);
  Duration renderdelta = static_cast<Duration>(rendertime.delta);
  TimePoint curtime = Clock::now();
  TimePoint prevtime = curtime - tickdelta;
  TimePoint nexttick = prevtime + tickdelta;
  TimePoint nextrender = prevtime + renderdelta;

  while (running) {
    if (nexttick < curtime) {
      update(ticktime.delta);
      nexttick += tickdelta;
    }

    if (nextrender < curtime) {
      draw(rendertime.delta);
      nextrender += renderdelta;
    }

    if (events.size() > 0
    or  states.size() > 0) {
      std::this_thread::sleep_until(std::min(nexttick, nextrender));
      prevtime = curtime;
      curtime = Clock::now();
    }
    else {
      quit();
    }
  }

  return true;
}

// TODO: move event processing to update function
void Engine::update(float delta_time) {
  if (window != nullptr) {
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

  if (events.size() > 0) {
    for (const StateEvent& event : events) {
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
  }

  for (BaseState* state : states) {
    state->update(delta_time);
  }
}

void Engine::draw(float delta_time) {
  for (BaseState* state : states) {
    state->draw(delta_time);
  }

  if (window != nullptr) {
    window->drawWindow();
  }
}

void Engine::quit() {
  if (window != nullptr) {
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
