#include "basegame.hpp"

#include "gameplay.hpp"
#include "../engine.hpp"

#include <sstream>

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

  Box mario_cb;
  mario_cb.radius = 6.f / 16.f;
  mario_cb.height = 24.f / 16.f;
  entity_data.registerDefault<CollisionBox>("player_mario", std::move(mario_cb));

  entity_data.registerDefault<Flags>("player_mario", Flags::GRAVITY);

  RenderStates mario_rs;
  mario_rs.pushFrame("idle", "bigmariowalk_0", Rect<int>(0, 0, 14, 27), Vec2f(7, -1), 0.f);

  mario_rs.pushFrame("walking", "bigmariowalk_1", Rect<int>(0, 0, 16, 27), Vec2f(9, -1), 0.125f);
  mario_rs.pushFrame("walking", "bigmariowalk_2", Rect<int>(0, 0, 16, 26), Vec2f(9, 0), 0.125f);
  mario_rs.pushFrame("walking", "bigmariowalk_1", Rect<int>(0, 0, 16, 27), Vec2f(9, -1), 0.125f);
  mario_rs.pushFrame("walking", "bigmariowalk_0", Rect<int>(0, 0, 14, 27), Vec2f(7, -1), 0.125f);

  mario_rs.pushFrame("running", "bigmariorun_1", Rect<int>(0, 0, 19, 27), Vec2f(11, -1), 1.f / 60.f);
  mario_rs.pushFrame("running", "bigmariorun_2", Rect<int>(0, 0, 19, 26), Vec2f(11, 0), 1.f / 60.f);
  mario_rs.pushFrame("running", "bigmariorun_1", Rect<int>(0, 0, 19, 27), Vec2f(11, -1), 1.f / 60.f);
  mario_rs.pushFrame("running", "bigmariorun_0", Rect<int>(0, 0, 19, 27), Vec2f(11, -1), 1.f / 60.f);

  mario_rs.pushFrame("slipping", "bigmarioslip", Rect<int>(0, 0, 16, 28), Vec2f(8, -1), 0.f);

  mario_rs.pushFrame("runjumping", "bigmariorunjump", Rect<int>(0, 0, 19, 26), Vec2f(11, 0), 0.f);

  mario_rs.pushFrame("ducking", "bigmarioduck", Rect<int>(0, 0, 14, 18), Vec2f(7, -1), 0.f);

  mario_rs.pushFrame("jumping", "bigmariojump", Rect<int>(0, 0, 16, 26), Vec2f(8, 0), 0.f);

  mario_rs.pushFrame("death", "mariodeath", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 0.f);
  entity_data.registerRenderStates("player_mario", std::move(mario_rs));
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
