#pragma once

#include "../engine.hpp"
#include "basestate.hpp"

namespace kme {
class WorldMap final : public BaseState {
public:
  static Factory create();

private:
  WorldMap(Engine* engine);

public:
  ~WorldMap() final = default;

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update(float delta) final;
  void draw(float delta) final;

private:
  bool paused = false;
};
}
