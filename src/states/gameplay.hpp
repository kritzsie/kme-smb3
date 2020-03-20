#pragma once

#include "basestate.hpp"
//#include "basegame/entities.hpp"
#include "basegame/world.hpp"
#include "basegame.hpp"
#include "../geometry.hpp"
#include "../types.hpp"

#include <SFML/Graphics.hpp>

#include <string>

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
  BaseGame* getBaseGame();

  void drawBackground(sf::Color color);
  void drawBackground(std::string texture);
  void drawTiles(Rect<Int32> region);

  Vec2f fromScreen(Vec2f pos);
  Vec2f toScreen(Vec2f pos);

  Vec2f fromTile(Vec2i pos);
  Vec2i toTile(Vec2f pos);

  float ticktime;
  float rendertime;

  sf::RenderTexture* framebuffer;

  Level level;
  UInt32 current_subworld;
};
}
