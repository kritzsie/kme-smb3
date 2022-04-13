#pragma once

#include "basestate.hpp"
//#include "basegame/entitydata.hpp"
#include "basegame/ecs/entitydata.hpp"
#include "basegame/tiledefs.hpp"

namespace kme {
class BaseGame final : public BaseState {
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

  void update(float delta) final;
  void draw(float delta) final;

private:
  bool paused = false;

public:
  EntityData entity_data;
  TileDefs level_tile_data;
};
}
