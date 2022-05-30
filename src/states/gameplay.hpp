#pragma once

#include "../graphics.hpp"
#include "../input.hpp"
#include "../inputhandler.hpp"
#include "../math.hpp"
#include "../types.hpp"
#include "basegame/theme.hpp"
#include "basegame/world.hpp"
#include "basestate.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <optional>
#include <string>

#include <cstddef>

namespace kme {
using namespace vec2_aliases;

class BaseGame;

class Gameplay final : public BaseState {
public:
  enum class Action {
    UP, LEFT, DOWN, RIGHT,
    JUMP, SPINJUMP, RUN,
    SELECT, PAUSE
  };

public:
  static Factory create(std::size_t worldnum, std::size_t levelnum);

private:
  Gameplay(BaseState* parent, Engine* engine,
           std::size_t worldnum, std::size_t levelnum);

public:
  bool handleInput(sf::Event::EventType type, const sf::Event& event) final;

  void enter() final;
  void exit() final;

  void pause() final;
  void resume() final;

  void update(float delta) final;
  void draw(float delta) final;

  bool playMusic(std::string name);
  void stopMusic();

  std::size_t playSound(std::string name);
  bool stopSound(std::size_t index);

  std::size_t playSoundLoop(std::string name);
  void stopSoundLoop(std::size_t index);

  void suspend();
  void unsuspend();
  bool isSuspended() const;

private:
  BaseGame* getBaseGame();

  void drawBackground(Color color);
  void drawBackground(Layer layer);
  // parallax is a factor from 0.0 to 1.0, NOT distance!
  void drawBackground(std::string name, Vec2f offset, Vec2f parallax_factor,
                      bool tile_vertically = false);
  void drawTiles();
  void drawEntities();
  void drawWater(float height);
  void drawHUD();

  static Vec2f fromScreen(Vec2f pos);
  static Vec2f toScreen(Vec2f pos);

  static Vec2f fromTile(Vec2i pos);
  static Vec2i toTile(Vec2f pos);

public:
  InputHandler<Action> inputs;

private:
  bool paused = false;

  bool suspended = false;
  bool suspended_previous = false;

  float ticktime = 0.f;
  float rendertime = 0.f;

  std::optional<sf::RenderTexture> framebuffer;
  std::optional<sf::RenderTexture> scene;
  std::optional<sf::RenderTexture> hud;

  std::size_t worldnum, levelnum;
  std::size_t current_subworld = 0;
  Level level;
};
}
