#include "basegame.hpp"

#include "basegame/ecs/components.hpp"
#include "gameplay.hpp"
#include "../engine.hpp"

#include <sstream>
#include <utility>

namespace kme {
BaseGame::Factory BaseGame::create() {
  return [](BaseState* parent, Engine* engine) -> BaseState* {
    return new BaseGame(parent, engine);
  };
}

void BaseGame::enter() {
  TileDef brick_block;
  brick_block.pushFrame("smb3_tile_atlas", Vec2i(0, 0), 8.f / 60.f);
  brick_block.pushFrame("smb3_tile_atlas", Vec2i(16, 0), 8.f / 60.f);
  brick_block.pushFrame("smb3_tile_atlas", Vec2i(32, 0), 8.f / 60.f);
  brick_block.pushFrame("smb3_tile_atlas", Vec2i(48, 0), 8.f / 60.f);
  level_tile_data.registerTileDef("brick_block", std::move(brick_block));

  TileDef item_block;
  item_block.pushFrame("smb3_tile_atlas", Vec2i(0, 48), 8.f / 60.f);
  item_block.pushFrame("smb3_tile_atlas", Vec2i(16, 48), 8.f / 60.f);
  item_block.pushFrame("smb3_tile_atlas", Vec2i(32, 48), 8.f / 60.f);
  item_block.pushFrame("smb3_tile_atlas", Vec2i(48, 48), 8.f / 60.f);
  level_tile_data.registerTileDef("item_block", std::move(item_block));

  TileDef wood_floors[6];
  wood_floors[0].pushFrame("smb3_tile_atlas", Vec2i(80, 48), 0.f);
  wood_floors[1].pushFrame("smb3_tile_atlas", Vec2i(96, 48), 0.f);
  wood_floors[2].pushFrame("smb3_tile_atlas", Vec2i(112, 48), 0.f);
  wood_floors[3].pushFrame("smb3_tile_atlas", Vec2i(80, 64), 0.f);
  wood_floors[4].pushFrame("smb3_tile_atlas", Vec2i(96, 64), 0.f);
  wood_floors[5].pushFrame("smb3_tile_atlas", Vec2i(112, 64), 0.f);
  for (std::size_t i = 0; i < 6; ++i) {
    std::stringstream ss;
    ss << "wood_floor_" << i;
    level_tile_data.registerTileDef(ss.str(), std::move(wood_floors[i]));
  }

  TileDef wood_block;
  wood_block.pushFrame("smb3_tile_atlas", Vec2i(208, 96), 0.f);
  level_tile_data.registerTileDef("wood_block", std::move(wood_block));

  TileDef gold_coin;
  gold_coin.setCollisionType(TileDef::CollisionType::NONE);
  gold_coin.pushFrame("smb3_tile_atlas", Vec2i(0, 32), 8.f / 60.f);
  gold_coin.pushFrame("smb3_tile_atlas", Vec2i(16, 32), 8.f / 60.f);
  gold_coin.pushFrame("smb3_tile_atlas", Vec2i(32, 32), 8.f / 60.f);
  gold_coin.pushFrame("smb3_tile_atlas", Vec2i(48, 32), 8.f / 60.f);
  level_tile_data.registerTileDef("gold_coin", std::move(gold_coin));

  CCollision mario_collision;
  mario_collision.radius = 4.f / 16.f;
  mario_collision.height = 25.f / 16.f;
  //entity_data.setDefault<CollisionComponent>("PlayerMario", std::move(mario_collision));

  //entity_data.setDefault<FlagsComponent>("PlayerMario", FlagsComponent::GRAVITY);

  RenderStates mario_rs;
  mario_rs.pushFrame("IDLE", "bigmariowalk_0", Rect<int>(0, 0, 14, 27), Vec2f(6, -1), 0.f);

  mario_rs.pushFrame("WALK", "bigmariowalk_1", Rect<int>(0, 0, 16, 27), Vec2f(8, -1), 0.125f);
  mario_rs.pushFrame("WALK", "bigmariowalk_2", Rect<int>(0, 0, 16, 26), Vec2f(8, 0), 0.125f);
  mario_rs.pushFrame("WALK", "bigmariowalk_1", Rect<int>(0, 0, 16, 27), Vec2f(8, -1), 0.125f);
  mario_rs.pushFrame("WALK", "bigmariowalk_0", Rect<int>(0, 0, 14, 27), Vec2f(6, -1), 0.125f);

  mario_rs.pushFrame("RUN", "bigmariorun_1", Rect<int>(0, 0, 19, 27), Vec2f(10, -1), 1.f / 60.f);
  mario_rs.pushFrame("RUN", "bigmariorun_2", Rect<int>(0, 0, 19, 26), Vec2f(10, 0), 1.f / 60.f);
  mario_rs.pushFrame("RUN", "bigmariorun_1", Rect<int>(0, 0, 19, 27), Vec2f(10, -1), 1.f / 60.f);
  mario_rs.pushFrame("RUN", "bigmariorun_0", Rect<int>(0, 0, 19, 27), Vec2f(10, -1), 1.f / 60.f);

  mario_rs.pushFrame("SLIP", "bigmarioslip", Rect<int>(0, 0, 16, 28), Vec2f(7, -1), 0.f);

  mario_rs.pushFrame("RUNJUMP", "bigmariorunjump", Rect<int>(0, 0, 19, 26), Vec2f(10, 0), 0.f);

  mario_rs.pushFrame("DUCK", "bigmarioduck", Rect<int>(0, 0, 14, 18), Vec2f(6, -1), 0.f);

  mario_rs.pushFrame("JUMP", "bigmariojump", Rect<int>(0, 0, 16, 26), Vec2f(7, 0), 0.f);

  mario_rs.pushFrame("DEATH", "mariodeath", Rect<int>(0, 0, 16, 16), Vec2f(7, -1), 0.f);
  entity_data.registerRenderStates("PlayerMario", std::move(mario_rs));
}

void BaseGame::exit() {}

void BaseGame::pause() {
  paused = true;
}

void BaseGame::resume() {
  paused = false;
}

void BaseGame::update(float delta) {
  if (not paused) {
    engine->pushState(Gameplay::create());
    pause();
  }
}

void BaseGame::draw(float delta) {}

BaseGame::BaseGame(BaseState* parent, Engine* engine) : BaseState(parent, engine) {}
}
