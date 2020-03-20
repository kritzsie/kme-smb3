#pragma once

#include "basestate.hpp"
#include "basegame/tiledefs.hpp"

namespace kme {
class BaseGame : public BaseState {
public:
  static Factory create();

private:
  BaseGame(BaseState* parent, Engine* engine);

public:
  ~BaseGame() final = default;

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

  TileDefs level_tiles;

private:
  bool paused = false;
};
}
