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

  TileDef gold_coin;
  gold_coin.setCollisionType(TileDef::CollisionType::NONE);
  gold_coin.pushFrame("smb3_tile_atlas", Vec2i(0, 32), 8.f / 60.f);
  gold_coin.pushFrame("smb3_tile_atlas", Vec2i(16, 32), 8.f / 60.f);
  gold_coin.pushFrame("smb3_tile_atlas", Vec2i(32, 32), 8.f / 60.f);
  gold_coin.pushFrame("smb3_tile_atlas", Vec2i(48, 32), 8.f / 60.f);
  level_tile_data.registerTileDef("gold_coin", std::move(gold_coin));

  TileDef item_block;
  item_block.pushFrame("smb3_tile_atlas", Vec2i(0, 48), 8.f / 60.f);
  item_block.pushFrame("smb3_tile_atlas", Vec2i(16, 48), 8.f / 60.f);
  item_block.pushFrame("smb3_tile_atlas", Vec2i(32, 48), 8.f / 60.f);
  item_block.pushFrame("smb3_tile_atlas", Vec2i(48, 48), 8.f / 60.f);
  level_tile_data.registerTileDef("item_block", std::move(item_block));

  TileDef bush;
  bush.setCollisionType(TileDef::CollisionType::NONE);
  bush.pushFrame("smb3_tile_atlas", Vec2i(400, 0), 0.f);
  level_tile_data.registerTileDef("bush", std::move(bush));

  TileDef hills1[4];
  hills1[0].pushFrame("smb3_tile_atlas", Vec2i(64, 0), 0.f);
  hills1[1].pushFrame("smb3_tile_atlas", Vec2i(80, 0), 0.f);
  hills1[2].pushFrame("smb3_tile_atlas", Vec2i(64, 16), 0.f);
  hills1[3].pushFrame("smb3_tile_atlas", Vec2i(80, 16), 0.f);
  for (std::size_t i = 0; i < 4; ++i) {
    std::stringstream ss;
    ss << "hills1_" << i;
    hills1[i].setCollisionType(TileDef::CollisionType::NONE);
    level_tile_data.registerTileDef(ss.str(), std::move(hills1[i]));
  }

  TileDef hills2[4];
  hills2[0].pushFrame("smb3_tile_atlas", Vec2i(96, 0), 0.f);
  hills2[1].pushFrame("smb3_tile_atlas", Vec2i(112, 0), 0.f);
  hills2[2].pushFrame("smb3_tile_atlas", Vec2i(96, 16), 0.f);
  hills2[3].pushFrame("smb3_tile_atlas", Vec2i(112, 16), 0.f);
  for (std::size_t i = 0; i < 4; ++i) {
    std::stringstream ss;
    ss << "hills2_" << i;
    hills2[i].setCollisionType(TileDef::CollisionType::NONE);
    level_tile_data.registerTileDef(ss.str(), std::move(hills2[i]));
  }

  TileDef hills3[2];
  hills3[0].pushFrame("smb3_tile_atlas", Vec2i(80, 32), 0.f);
  hills3[1].pushFrame("smb3_tile_atlas", Vec2i(96, 32), 0.f);
  for (std::size_t i = 0; i < 2; ++i) {
    std::stringstream ss;
    ss << "hills3_" << i;
    hills3[i].setCollisionType(TileDef::CollisionType::NONE);
    level_tile_data.registerTileDef(ss.str(), std::move(hills3[i]));
  }

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
  wood_block.pushFrame("smb3_tile_atlas", Vec2i(64, 64), 0.f);
  level_tile_data.registerTileDef("wood_block", std::move(wood_block));

  TileDef cloud_platform;
  cloud_platform.setCollisionType(TileDef::CollisionType::PLATFORM);
  cloud_platform.pushFrame("smb3_tile_atlas", Vec2i(368, 0), 0.f);
  level_tile_data.registerTileDef("cloud_platform", std::move(cloud_platform));

  TileDef green_pipes[4];
  green_pipes[0].pushFrame("smb3_tile_atlas", Vec2i(64, 112), 0.f);
  green_pipes[1].pushFrame("smb3_tile_atlas", Vec2i(80, 112), 0.f);
  green_pipes[2].pushFrame("smb3_tile_atlas", Vec2i(64, 128), 0.f);
  green_pipes[3].pushFrame("smb3_tile_atlas", Vec2i(80, 128), 0.f);
  for (std::size_t i = 0; i < 4; ++i) {
    std::stringstream ss;
    ss << "green_pipe_" << i;
    level_tile_data.registerTileDef(ss.str(), std::move(green_pipes[i]));
  }

  TileDef orange_platforms[12];
  orange_platforms[0].pushFrame("smb3_tile_atlas", Vec2i(128, 0), 0.f);
  orange_platforms[1].pushFrame("smb3_tile_atlas", Vec2i(144, 0), 0.f);
  orange_platforms[2].pushFrame("smb3_tile_atlas", Vec2i(160, 0), 0.f);
  orange_platforms[3].pushFrame("smb3_tile_atlas", Vec2i(176, 0), 0.f);
  orange_platforms[4].pushFrame("smb3_tile_atlas", Vec2i(128, 16), 0.f);
  orange_platforms[5].pushFrame("smb3_tile_atlas", Vec2i(144, 16), 0.f);
  orange_platforms[6].pushFrame("smb3_tile_atlas", Vec2i(160, 16), 0.f);
  orange_platforms[7].pushFrame("smb3_tile_atlas", Vec2i(176, 16), 0.f);
  orange_platforms[8].pushFrame("smb3_tile_atlas", Vec2i(128, 32), 0.f);
  orange_platforms[9].pushFrame("smb3_tile_atlas", Vec2i(144, 32), 0.f);
  orange_platforms[10].pushFrame("smb3_tile_atlas", Vec2i(160, 32), 0.f);
  orange_platforms[11].pushFrame("smb3_tile_atlas", Vec2i(176, 32), 0.f);
  for (std::size_t i = 0; i < 12; ++i) {
    std::stringstream ss;
    ss << "orange_platform_" << i;
    orange_platforms[i].setCollisionType(i < 3 ? TileDef::CollisionType::PLATFORM : TileDef::CollisionType::NONE);
    level_tile_data.registerTileDef(ss.str(), std::move(orange_platforms[i]));
  }

  TileDef green_platforms[12];
  green_platforms[0].pushFrame("smb3_tile_atlas", Vec2i(192, 0), 0.f);
  green_platforms[1].pushFrame("smb3_tile_atlas", Vec2i(208, 0), 0.f);
  green_platforms[2].pushFrame("smb3_tile_atlas", Vec2i(224, 0), 0.f);
  green_platforms[3].pushFrame("smb3_tile_atlas", Vec2i(240, 0), 0.f);
  green_platforms[4].pushFrame("smb3_tile_atlas", Vec2i(192, 16), 0.f);
  green_platforms[5].pushFrame("smb3_tile_atlas", Vec2i(208, 16), 0.f);
  green_platforms[6].pushFrame("smb3_tile_atlas", Vec2i(224, 16), 0.f);
  green_platforms[7].pushFrame("smb3_tile_atlas", Vec2i(240, 16), 0.f);
  green_platforms[8].pushFrame("smb3_tile_atlas", Vec2i(192, 32), 0.f);
  green_platforms[9].pushFrame("smb3_tile_atlas", Vec2i(208, 32), 0.f);
  green_platforms[10].pushFrame("smb3_tile_atlas", Vec2i(224, 32), 0.f);
  green_platforms[11].pushFrame("smb3_tile_atlas", Vec2i(240, 32), 0.f);
  for (std::size_t i = 0; i < 12; ++i) {
    std::stringstream ss;
    ss << "green_platform_" << i;
    green_platforms[i].setCollisionType(i < 3 ? TileDef::CollisionType::PLATFORM : TileDef::CollisionType::NONE);
    level_tile_data.registerTileDef(ss.str(), std::move(green_platforms[i]));
  }

  TileDef blue_platforms[12];
  blue_platforms[0].pushFrame("smb3_tile_atlas", Vec2i(128, 48), 0.f);
  blue_platforms[1].pushFrame("smb3_tile_atlas", Vec2i(144, 48), 0.f);
  blue_platforms[2].pushFrame("smb3_tile_atlas", Vec2i(160, 48), 0.f);
  blue_platforms[3].pushFrame("smb3_tile_atlas", Vec2i(176, 48), 0.f);
  blue_platforms[4].pushFrame("smb3_tile_atlas", Vec2i(128, 64), 0.f);
  blue_platforms[5].pushFrame("smb3_tile_atlas", Vec2i(144, 64), 0.f);
  blue_platforms[6].pushFrame("smb3_tile_atlas", Vec2i(160, 64), 0.f);
  blue_platforms[7].pushFrame("smb3_tile_atlas", Vec2i(176, 64), 0.f);
  blue_platforms[8].pushFrame("smb3_tile_atlas", Vec2i(128, 80), 0.f);
  blue_platforms[9].pushFrame("smb3_tile_atlas", Vec2i(144, 80), 0.f);
  blue_platforms[10].pushFrame("smb3_tile_atlas", Vec2i(160, 80), 0.f);
  blue_platforms[11].pushFrame("smb3_tile_atlas", Vec2i(176, 80), 0.f);
  for (std::size_t i = 0; i < 12; ++i) {
    std::stringstream ss;
    ss << "blue_platform_" << i;
    blue_platforms[i].setCollisionType(i < 3 ? TileDef::CollisionType::PLATFORM : TileDef::CollisionType::NONE);
    level_tile_data.registerTileDef(ss.str(), std::move(blue_platforms[i]));
  }

  TileDef gray_platforms[12];
  gray_platforms[0].pushFrame("smb3_tile_atlas", Vec2i(192, 48), 0.f);
  gray_platforms[1].pushFrame("smb3_tile_atlas", Vec2i(208, 48), 0.f);
  gray_platforms[2].pushFrame("smb3_tile_atlas", Vec2i(224, 48), 0.f);
  gray_platforms[3].pushFrame("smb3_tile_atlas", Vec2i(240, 48), 0.f);
  gray_platforms[4].pushFrame("smb3_tile_atlas", Vec2i(192, 64), 0.f);
  gray_platforms[5].pushFrame("smb3_tile_atlas", Vec2i(208, 64), 0.f);
  gray_platforms[6].pushFrame("smb3_tile_atlas", Vec2i(224, 64), 0.f);
  gray_platforms[7].pushFrame("smb3_tile_atlas", Vec2i(240, 64), 0.f);
  gray_platforms[8].pushFrame("smb3_tile_atlas", Vec2i(192, 80), 0.f);
  gray_platforms[9].pushFrame("smb3_tile_atlas", Vec2i(208, 80), 0.f);
  gray_platforms[10].pushFrame("smb3_tile_atlas", Vec2i(224, 80), 0.f);
  gray_platforms[11].pushFrame("smb3_tile_atlas", Vec2i(240, 80), 0.f);
  for (std::size_t i = 0; i < 12; ++i) {
    std::stringstream ss;
    ss << "gray_platform_" << i;
    gray_platforms[i].setCollisionType(i < 3 ? TileDef::CollisionType::PLATFORM : TileDef::CollisionType::NONE);
    level_tile_data.registerTileDef(ss.str(), std::move(gray_platforms[i]));
  }

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
