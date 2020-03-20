#pragma once

#include "geometry.hpp"
#include "input.hpp"
#include "states.hpp"
#include "types.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <cstddef>

namespace kme {
using Clock = std::chrono::steady_clock;
using Duration = std::chrono::duration<float>;
using TimePoint = std::chrono::time_point<Clock, Duration>;

using TextureMap = std::map<std::string, std::shared_ptr<sf::Texture>>;

struct PhysFSInfo {
  std::string prefdir;
};

class TimeInfo {
public:
  TimeInfo(float rate);

  float rate;
  float delta;
};

class Window : public sf::RenderWindow {
public:
  Window();
  Window(UInt32 width, UInt32 height);
  Window(UInt32 width, UInt32 height, const char* title);
  ~Window();

  void update();
  void resize(UInt32 width, UInt32 height);

  sf::RenderTexture* getFramebuffer();

private:
  sf::RenderTexture* framebuffer;
};

class Engine {
public:
  enum class StateEventType {
    PUSH,
    POP
  };

  using StateEvent = std::pair<StateEventType, BaseState::Factory>;

  Engine(int, char**);
  Engine(std::vector<std::string>&& args);
  ~Engine();

public:
  int exec();

  Window* getWindow();

  bool isRunning() const;
  TimeInfo getTickTime() const;
  TimeInfo getRenderTime() const;

  void pushState(BaseState::Factory);
  BaseState* popState();

protected:
  bool main();
  void update();
  void draw();
  void quit();

  std::vector<std::string> args;

private:
  bool setupPhysFS();

  static std::size_t instance_count;

  bool running;

  TimeInfo ticktime;
  TimeInfo rendertime;
  PhysFSInfo physfsinfo;

  Window* window;

  std::vector<StateEvent> events;
  std::vector<BaseState*> states;
};
}
