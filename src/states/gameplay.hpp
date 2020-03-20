#pragma once

#include "basestate.hpp"
//#include "basegame/entities.hpp"
#include "basegame/world.hpp"
#include "basegame.hpp"
#include "../geometry.hpp"
#include "../types.hpp"

#include <SFML/Graphics.hpp>

namespace kme {
class Gameplay : public BaseState {
public:
  static Factory create();

private:
  Gameplay(BaseState* parent, Engine* engine);

public:
  ~Gameplay() final;

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update() final;
  void draw() final;

private:
  void drawTiles(Rect<Int32> region);

  float ticktime;
  float rendertime;

  BaseGame* base_game;
  sf::RenderTexture* framebuffer;

  UInt32 subworld_current;
  Level level;
};
}
