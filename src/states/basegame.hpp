#pragma once

#include "../engine.hpp"
#include "../inputhandler.hpp"
#include "../renderstates.hpp"
#include "../types.hpp"
#include "basegame/ecs/entitydefs.hpp"
#include "basegame/entity.hpp"
#include "basegame/theme.hpp"
#include "basegame/tiledefs.hpp"
#include "basestate.hpp"

#include <functional>
#include <unordered_map>

namespace kme {
class BaseGame final : public BaseState {
public:
  using Spawner = std::function<Entity (Vec2f)>;
  using SpawnHandler = std::function<void (EntityRegistry&, Entity)>;

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
  void addScore(long count);
  void addScore(ULong count);

  Spawner getSpawner(EntityRegistry& entities, EntityType entity_type);

private:
  bool paused = false;

public:
  std::unordered_map<EntityType, SpawnHandler> entity_spawn_data;

  EntityDefs entity_data;
  TileDefs level_tile_data;

  StringTable<RenderFrames> backgrounds;
  StringTable<Theme> themes;

private:
  UByte coins = 0;
  UByte lives = 5;
  ULong score = 0;
};
}
