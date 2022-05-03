#pragma once

#include "basestate.hpp"

#include "../renderstates.hpp"
#include "../types.hpp"
#include "basegame/ecs/entitydata.hpp"
#include "basegame/theme.hpp"
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

  UInt getCoins() const;
  UInt getLives() const;
  ULong getScore() const;

  void addCoins(int count);
  void addLives(int count);
  void addScore(int count);
  void addScore(long count);
  void addScore(ULong count);

private:
  bool paused = false;

public:
  EntityData entity_data;
  TileDefs level_tile_data;

  StringTable<RenderFrames> backgrounds;
  StringTable<Theme> themes;

private:
  UByte coins = 0;
  UByte lives = 5;
  ULong score = 0;
};
}
