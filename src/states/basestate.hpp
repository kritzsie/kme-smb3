#pragma once

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

  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;

  virtual void update() = 0;
  virtual void draw() = 0;

protected:
  BaseState* const parent;
  Engine* const engine;
};
}
