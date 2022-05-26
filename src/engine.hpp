#pragma once

#include "math.hpp"
#include "music.hpp"
#include "sound.hpp"
#include "states.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <cstddef>

namespace kme {
using Clock = std::chrono::steady_clock;
using Duration = std::chrono::duration<float>;
using TimePoint = std::chrono::time_point<Clock, Duration>;

struct PhysFSInfo {
  std::string prefdir;
};

struct TimeInfo {
public:
  TimeInfo(float rate);

  float rate;
  float delta;
};

class Window : public sf::RenderWindow {
public:
  Window();
  Window(std::size_t width, std::size_t height, std::string title = "Klaymore Engine");

  void drawWindow();
  void resize(std::size_t width, std::size_t height);

  std::optional<sf::RenderTexture>& getFramebuffer();

private:
  std::optional<sf::RenderTexture> framebuffer;
};

class Engine {
public:
  enum class StateEventType { PUSH, POP };

  using StateEvent = std::pair<StateEventType, BaseState::Factory>;

  Engine(int argc, char** argv);
  Engine(StringList args);
  ~Engine();

public:
  int exec();

  std::optional<Window>& getWindow();

  bool isRunning() const;
  TimeInfo getTickTime() const;
  TimeInfo getRenderTime() const;

  void pushState(BaseState::Factory factory);
  BaseState* popState();

protected:
  bool main();
  void update(float delta);
  void draw(float delta);
  void quit();

private:
  bool setupPhysFS();

protected:
  std::vector<std::string> args;

private:
  static std::size_t instance_count;

  bool running;

  TimeInfo tickinfo;
  TimeInfo renderinfo;
  PhysFSInfo physfsinfo;

  std::optional<Window> window;

public:
  std::optional<Music> music;
  std::optional<Sound> sound;

private:
  std::vector<StateEvent> events;
  std::vector<BaseState*> states;
};
}
