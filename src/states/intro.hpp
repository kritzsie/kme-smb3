#pragma once

#include "basestate.hpp"

namespace kme {
class Intro : public BaseState {
public:
  static Factory create();

private:
  Intro(Engine* engine);

public:
  ~Intro() final = default;

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

private:
  bool paused = false;
};
}
