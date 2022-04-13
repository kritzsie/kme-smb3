#pragma once

#include "basestate.hpp"
#include "basegame/world.hpp"
#include "basegame.hpp"
#include "../geometry.hpp"
#include "../input.hpp"

#include <entt/entt.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

namespace kme {
class Gameplay final : public BaseState {
public:
  static Factory create();

  enum class Action {
    UP, LEFT, DOWN, RIGHT,
    JUMP, SPINJUMP, RUN,
    SELECT, PAUSE
  };

  using BindMap = std::map<sf::Keyboard::Key, Action>;
  using ButtonMap = std::map<Action, Button>;

private:
  Gameplay(BaseState* parent, Engine* engine);

public:
  ~Gameplay() final;

  bool handleInput(sf::Event::EventType type, const sf::Event& event) final;

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update(float delta) final;
  void draw(float delta) final;

private:
  BaseGame* getBaseGame();

  void drawBackground(sf::Color color);
  // parallax here is a factor from 0.0 to 1.0, not distance
  void drawBackground(std::string texture, Vec2f offset, Vec2f parallax,
                      bool vertical_tiling = false);
  void drawTiles();
  void drawEntities();

  Vec2f fromScreen(Vec2f pos);
  Vec2f toScreen(Vec2f pos);

  Vec2f fromTile(Vec2i pos);
  Vec2i toTile(Vec2f pos);

  float ticktime = 0.f;
  float rendertime = 0.f;

  sf::RenderTexture* framebuffer = nullptr;

  Vec2f camera_pos;
  Vec2f camera_radius;

  std::size_t current_subworld = 0;
  Level level;

public:
  BindMap binds;
  ButtonMap buttons;
};
}
