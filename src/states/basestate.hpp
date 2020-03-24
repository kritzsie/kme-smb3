#pragma once

#include <SFML/Window/Event.hpp>

#include <functional>

namespace kme {
class Engine;

class BaseState {
public:
  using Factory = std::function<BaseState* (BaseState*, Engine*)>;

protected:
  BaseState(BaseState* parent, Engine* engine);

public:
  virtual ~BaseState() = default;

  virtual bool handleInput(sf::Event::EventType event_type, const sf::Event& event);

  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;

  virtual void update(float delta) = 0;
  virtual void draw(float delta) = 0;

protected:
  BaseState* const parent;
  Engine* const engine;
};
}
