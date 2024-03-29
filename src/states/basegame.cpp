#include "basegame.hpp"

#include "../engine.hpp"
#include "../graphics.hpp"
#include "../math.hpp"
#include "basegame/ecs/components.hpp"
#include "basegame/gameloader.hpp"
#include "basegame/hitbox.hpp"
#include "basegame/powerup.hpp"
#include "basegame/theme.hpp"
#include "worldmap.hpp"

#include <map>
#include <sstream>
#include <utility>

namespace kme {
using namespace vec2_aliases;

BaseGame::Factory BaseGame::create() {
  return [](BaseState* parent, Engine* engine) -> BaseState* {
    return new BaseGame(parent, engine);
  };
}

BaseGame::BaseGame(BaseState* parent, Engine* engine) : BaseState(parent, engine) {}

BaseGame::Spawner BaseGame::getSpawner(EntityRegistry& entities, EntityType entity_type) {
  return [this, &entities, entity_type](Vec2f pos) -> Entity {
    auto entity = entities.create();
    entities.emplace<CInfo>(entity, entity_type);
    entities.emplace<CPosition>(entity, pos);
    entity_spawn_data.at(entity_type)(entities, entity);
    return entity;
  };
}

void BaseGame::enter() {
  TileDefLoader loader;
  loader.load(level_tile_data);

  entity_spawn_data["Player"] = [this](EntityRegistry& entities, Entity entity) {
    EntityType type = "Player";
    auto powerup = Powerup::NONE;
    auto state = EState::IDLE;
    auto& hitbox = entity_data.getHitboxes(type).at(powerup).at(state);
    entities.emplace<CPowerup>(entity, powerup);
    entities.emplace<CState>(entity, state);
    entities.emplace<CCollision>(entity, hitbox);
    entities.emplace<CVelocity>(entity);
    entities.emplace<CDirection>(entity);
    entities.emplace<CFlags>(entity);
    entities.emplace<CCounters>(entity);
    entities.emplace<CTimers>(entity);
    entities.emplace<CRender>(entity);
    entities.emplace<CAudio>(entity);
  };

  entity_spawn_data["Camera"] = [](EntityRegistry& entities, Entity entity) {
    entities.emplace<CCollision>(entity, Hitbox(15.f, 16.875f));
  };

  entity_spawn_data["PlayerStart"] = [this](EntityRegistry& entities, Entity entity) {
    auto& pos = entities.get<CPosition>(entity).value;
    auto sign_spawner = getSpawner(entities, "StartSign");
    sign_spawner(pos);
  };

  entity_spawn_data["StartSign"] = [](EntityRegistry& entities, Entity entity) {
    entities.emplace<CRender>(entity);
  };

  entity_spawn_data["Checkpoint"] = [](EntityRegistry& entities, Entity entity) {
  };

  entity_spawn_data["GoalCard"] = [](EntityRegistry& entities, Entity entity) {
    entities.emplace<CCollision>(entity, Hitbox(0.5f, 1.f));
    entities.emplace<CFlags>(entity);
    entities.emplace<CState>(entity);
    entities.emplace<CRender>(entity);
  };

  entity_spawn_data["Mushroom"] = [](EntityRegistry& entities, Entity entity) {
    entities.emplace<CFlags>(entity, EFlags::POWERUP | EFlags::NOFRICTION);
    entities.emplace<CVelocity>(entity, Vec2f(4.f, 0.f));
    entities.emplace<CCollision>(entity, Hitbox(0.5f, 1.f));
    entities.emplace<CPowerup>(entity, Powerup::MUSHROOM);
    entities.emplace<CDirection>(entity);
    entities.emplace<CState>(entity);
    entities.emplace<CRender>(entity);
  };

  entity_spawn_data["Checkpoint"] = [](EntityRegistry& entities, Entity entity) {
  };

  entity_spawn_data["Goomba"] = [](EntityRegistry& entities, Entity entity) {
    entities.emplace<CFlags>(entity, EFlags::ENEMY | EFlags::NOFRICTION);
    entities.emplace<CState>(entity, EState::WALK);
    entities.emplace<CVelocity>(entity, Vec2f(-2.f, 0.f));
    entities.emplace<CCollision>(entity, Hitbox(0.5f, 0.75f));
    entities.emplace<CDirection>(entity);
    entities.emplace<CTimers>(entity);
    entities.emplace<CRender>(entity);
  };

  entity_spawn_data["RaccoonLeaf"] = [](EntityRegistry& entities, Entity entity) {
  };

  entity_spawn_data["PSwitch"] = [](EntityRegistry& entities, Entity entity) {
    entities.emplace<CInfo>(entity, "PSwitch");
    entities.emplace<CPosition>(entity, Vec2f(16.5f, 4.f));
    entities.emplace<CCollision>(entity, Hitbox(0.5f, 1.f));
    entities.emplace<CFlags>(entity, EFlags::SOLID);
    entities.emplace<CVelocity>(entity);
    entities.emplace<CState>(entity);
    entities.emplace<CRender>(entity);
  };

  EntityDefs::Hitboxes mario_hb;
  mario_hb[Powerup::NONE][EState::IDLE] = Hitbox(6.f / 16.f, 15.f / 16.f);
  mario_hb[Powerup::MUSHROOM][EState::IDLE] = Hitbox(6.f / 16.f, 25.f / 16.f);
  mario_hb[Powerup::MUSHROOM][EState::DUCK] = Hitbox(6.f / 16.f, 15.f / 16.f);
  entity_data.registerHitboxes("Player", std::move(mario_hb));

  RenderStates mario_rs;
  mario_rs.pushFrame("IDLE", "smallmariowalk_0", Rect<int>(0, 0, 12, 15), Vec2f(6, -1), 0.f);

  mario_rs.pushFrame("WALK", "smallmariowalk_1", Rect<int>(0, 0, 15, 16), Vec2f(8, -1), 8.f / 60.f);
  mario_rs.pushFrame("WALK", "smallmariowalk_0", Rect<int>(0, 0, 12, 15), Vec2f(6, -1), 8.f / 60.f);

  mario_rs.pushFrame("RUN", "smallmariorun_1", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 1.f / 60.f);
  mario_rs.pushFrame("RUN", "smallmariorun_0", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 1.f / 60.f);

  mario_rs.pushFrame("JUMP", "smallmariojump", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 0.f);

  mario_rs.pushFrame("RUNJUMP", "smallmariorunjump", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 0.f);

  mario_rs.pushFrame("SLIP", "smallmarioslip", Rect<int>(0, 0, 14, 16), Vec2f(7, -1), 0.f);

  mario_rs.pushFrame("UNDERWATER", "player1swim_0", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 16.f / 60.f);
  mario_rs.pushFrame("UNDERWATER", "player1swim_1", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 16.f / 60.f);

  mario_rs.pushFrame("SWIM", "player1swim_0", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 8.f / 60.f);
  mario_rs.pushFrame("SWIM", "player1paddle_0", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 8.f / 60.f);
  mario_rs.pushFrame("SWIM", "player1paddle_1", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 8.f / 60.f);

  mario_rs.pushFrame("DEATH", "mariodeath", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 0.f);

  mario_rs.pushFrame("IDLE.BIG", "bigmariowalk_0", Rect<int>(0, 0, 14, 27), Vec2f(6, -1), 0.f);

  mario_rs.pushFrame("WALK.BIG", "bigmariowalk_1", Rect<int>(0, 0, 16, 27), Vec2f(8, -1), 8.f / 60.f);
  mario_rs.pushFrame("WALK.BIG", "bigmariowalk_2", Rect<int>(0, 0, 16, 26), Vec2f(8, 0), 8.f / 60.f);
  mario_rs.pushFrame("WALK.BIG", "bigmariowalk_1", Rect<int>(0, 0, 16, 27), Vec2f(8, -1), 8.f / 60.f);
  mario_rs.pushFrame("WALK.BIG", "bigmariowalk_0", Rect<int>(0, 0, 14, 27), Vec2f(6, -1), 8.f / 60.f);

  mario_rs.pushFrame("RUN.BIG", "bigmariorun_1", Rect<int>(0, 0, 19, 27), Vec2f(10, -1), 1.f / 60.f);
  mario_rs.pushFrame("RUN.BIG", "bigmariorun_2", Rect<int>(0, 0, 19, 26), Vec2f(10, 0), 1.f / 60.f);
  mario_rs.pushFrame("RUN.BIG", "bigmariorun_1", Rect<int>(0, 0, 19, 27), Vec2f(10, -1), 1.f / 60.f);
  mario_rs.pushFrame("RUN.BIG", "bigmariorun_0", Rect<int>(0, 0, 19, 27), Vec2f(10, -1), 1.f / 60.f);

  mario_rs.pushFrame("JUMP.BIG", "bigmariojump", Rect<int>(0, 0, 16, 26), Vec2f(7, 0), 0.f);

  mario_rs.pushFrame("RUNJUMP.BIG", "bigmariorunjump", Rect<int>(0, 0, 19, 26), Vec2f(10, 0), 0.f);

  mario_rs.pushFrame("SLIP.BIG", "bigmarioslip", Rect<int>(0, 0, 16, 28), Vec2f(7, -1), 0.f);

  mario_rs.pushFrame("DUCK.BIG", "bigmarioduck", Rect<int>(0, 0, 14, 18), Vec2f(6, -1), 0.f);

  mario_rs.pushFrame("UNDERWATER.BIG", "player/swim_big_p1_0",
                     Rect<int>(0, 0, 32, 32), Vec2f(16, -1), 16.f / 60.f);
  mario_rs.pushFrame("UNDERWATER.BIG", "player/swim_big_p1_1",
                     Rect<int>(0, 0, 32, 32), Vec2f(16, -1), 16.f / 60.f);
  mario_rs.pushFrame("UNDERWATER.BIG", "player/swim_big_p1_2",
                     Rect<int>(0, 0, 32, 32), Vec2f(16, -1), 16.f / 60.f);

  mario_rs.pushFrame("SWIM.BIG", "player/swim_big_p1_0", Rect<int>(0, 0, 32, 32), Vec2f(16, -1), 8.f / 60.f);
  mario_rs.pushFrame("SWIM.BIG", "player/paddle_big_p1_0", Rect<int>(0, 0, 32, 32), Vec2f(16, -1), 8.f / 60.f);
  mario_rs.pushFrame("SWIM.BIG", "player/paddle_big_p1_1", Rect<int>(0, 0, 32, 32), Vec2f(16, -1), 8.f / 60.f);

  entity_data.registerRenderStates("Player", std::move(mario_rs));

  RenderStates startsign_rs;
  startsign_rs.pushFrame("IDLE", "decoration/sign_start",
                         Rect<int>(0, 0, 64, 64), Vec2f(32, 0), 0.f);
  entity_data.registerRenderStates("StartSign", std::move(startsign_rs));

  RenderStates goalcard_rs;
  goalcard_rs.pushFrame("IDLE", "redmushroom", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 8.f / 60.f);
  goalcard_rs.pushFrame("IDLE", "fireflower", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 8.f / 60.f);
  goalcard_rs.pushFrame("IDLE", "superstar_0", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 8.f / 60.f);
  entity_data.registerRenderStates("GoalCard", std::move(goalcard_rs));

  RenderStates goomba_rs;
  goomba_rs.pushFrame("WALK", "goombawalk_0", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 8.f / 60.f);
  goomba_rs.pushFrame("WALK", "goombawalk_1", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 8.f / 60.f);

  goomba_rs.pushFrame("DEATH", "goombastomp", Rect<int>(0, 0, 16, 9), Vec2f(8, -1), 0.f);
  entity_data.registerRenderStates("Goomba", std::move(goomba_rs));

  RenderStates mushroom_rs;
  mushroom_rs.pushFrame("IDLE.BIG", "redmushroom", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 0.f);
  mushroom_rs.pushFrame("IDLE.FIRE", "fireflower", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 0.f);
  mushroom_rs.pushFrame("IDLE.RACCOON", "raccoonleaf", Rect<int>(0, 0, 16, 16), Vec2f(8, -1), 0.f);
  entity_data.registerRenderStates("Mushroom", std::move(mushroom_rs));

  RenderStates pswitch_rs;
  pswitch_rs.pushFrame("IDLE", "pswitch_0", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 8.f / 60.f);
  pswitch_rs.pushFrame("IDLE", "pswitch_1", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 8.f / 60.f);
  pswitch_rs.pushFrame("IDLE", "pswitch_0", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 8.f / 60.f);
  pswitch_rs.pushFrame("IDLE", "pswitch_2", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 8.f / 60.f);

  pswitch_rs.pushFrame("DEATH", "pswitchspent", Rect<int>(0, 0, 16, 16), Vec2f(8, 0), 0.f);
  entity_data.registerRenderStates("PSwitch", std::move(pswitch_rs));

  RenderFrames cloudlayer;
  cloudlayer.pushFrame("cloudlayer", Rect<int>(0, 0, 256, 256), Vec2f(), 0.f);
  backgrounds["cloudlayer"] = std::move(cloudlayer);

  RenderFrames overworldblockstop;
  overworldblockstop.pushFrame("overworldblockstop", Rect<int>(0, 0, 512, 192), Vec2f(), 0.f);
  backgrounds["overworldblockstop"] = std::move(overworldblockstop);

  RenderFrames athletichills;
  athletichills.pushFrame("athletichills", Rect<int>(0, 0, 512, 256), Vec2f(), 0.f);
  backgrounds["athletichills"] = std::move(athletichills);

  RenderFrames overworldcavefront;
  overworldcavefront.pushFrame("overworldcavefront", Rect<int>(0, 0, 512, 384), Vec2f(), 0.f);
  backgrounds["overworldcavefront"] = std::move(overworldcavefront);

  RenderFrames overworldcaveback;
  overworldcaveback.pushFrame("overworldcaveback", Rect<int>(0, 0, 256, 256), Vec2f(), 0.f);
  backgrounds["overworldcaveback"] = std::move(overworldcaveback);

  RenderFrames waterworld;
  waterworld.pushFrame("waterworld", Rect<int>(0, 0, 512, 384), Vec2f(), 0.f);
  backgrounds["waterworld"] = std::move(waterworld);

  RenderFrames fortressfront;
  fortressfront.pushFrame("fortressfront", Rect<int>(0, 0, 512, 432), Vec2f(), 0.f);
  backgrounds["fortressfront"] = std::move(fortressfront);

  RenderFrames fortressback;
  fortressback.pushFrame("fortressback", Rect<int>(0, 0, 256, 256), Vec2f(), 0.f);
  backgrounds["fortressback"] = std::move(fortressback);

  RenderFrames bonusquestion;
  bonusquestion.pushFrame("bonusquestion", Rect<int>(0, 0, 64, 64), Vec2f(), 0.f);
  backgrounds["bonusquestion"] = std::move(bonusquestion);

  RenderFrames pipesback;
  pipesback.pushFrame("pipes", Rect<int>(0, 0, 256, 512), Vec2f(), 0.f);
  backgrounds["pipesback"] = std::move(pipesback);

  RenderFrames pipesfront;
  pipesfront.pushFrame("", Rect<int>(), Vec2f(), 8.f / 60.f);
  pipesfront.pushFrame("pipes_1", Rect<int>(0, 0, 256, 512), Vec2f(), 8.f / 60.f);
  pipesfront.pushFrame("pipes_2", Rect<int>(0, 0, 256, 512), Vec2f(), 8.f / 60.f);
  backgrounds["pipesfront"] = std::move(pipesfront);

  Theme overworld_blocks;
  overworld_blocks.background = Color(0x6898F8FF);
  overworld_blocks.layers[0] = {
    .background = "overworldblockstop",
    .offset = Vec2f(128.f, -11.f),
    .parallax = Vec2f(0.375f, 0.25f),
    .repeat_y = false
  };
  overworld_blocks.layers[1] = {
    .background = "cloudlayer",
    .offset = Vec2f(0.f, 224.f),
    .parallax = Vec2f(0.75f, 0.125f),
    .repeat_y = false
  };
  overworld_blocks.music = "overworld.spc";
  themes["overworld_blocks"] = std::move(overworld_blocks);

  Theme athletic_hills;
  athletic_hills.background = Color(0x6898F8FF);
  athletic_hills.layers[-1] = {
    .background = "athletichills",
    .offset = Vec2f(128.f, -11.f),
    .parallax = Vec2f(0.375f, 0.25f),
    .repeat_y = false
  };
  athletic_hills.layers[0] = {
    .background = "cloudlayer",
    .offset = Vec2f(0.f, 224.f),
    .parallax = Vec2f(0.75f, 0.125f),
    .repeat_y = false
  };
  athletic_hills.music = "athletic.spc";
  themes["athletic_hills"] = std::move(athletic_hills);

  Theme overworld_cave;
  overworld_cave.background = Color(0x6898F8FF);
  overworld_cave.layers[-1] = {
    .background = "overworldcaveback",
    .offset = Vec2f(0.f, -68.f),
    .parallax = Vec2f(0.75f, 0.f),
    .repeat_y = false
  };
  overworld_cave.layers[0] = {
    .background = "overworldcavefront",
    .offset = Vec2f(0.f, -5.f),
    .parallax = Vec2f(0.375f, 0.f),
    .repeat_y = false
  };
  overworld_cave.music = "underworld.spc";
  themes["overworld_cave"] = std::move(overworld_cave);

  Theme water;
  water.background = Color(0x080060FF);
  water.layers[0] = {
    .background = "waterworld",
    .offset = Vec2f(0.f, -11.f),
    .parallax = Vec2f(0.375f, 0.f),
    .repeat_y = false
  };
  water.music = "swimming.spc";
  themes["water"] = std::move(water);

  Theme pipes;
  pipes.background = Color();
  pipes.layers[-1] = {
    .background = "pipesback",
    .offset = Vec2f(0.f, 0.f),
    .parallax = Vec2f(0.375f, 0.375f),
    .repeat_y = true
  };
  pipes.layers[0] = {
    .background = "pipesfront",
    .offset = Vec2f(0.f, 0.f),
    .parallax = Vec2f(0.375f, 0.375f),
    .repeat_y = true
  };
  pipes.music = "underworld.spc";
  themes["pipes"] = std::move(pipes);

  Theme fortress;
  fortress.background = Color();
  fortress.layers[-1] = {
    .background = "fortressback",
    .offset = Vec2f(0.f, -62.f),
    .parallax = Vec2f(0.75f, 0.f),
    .repeat_y = false
  };
  fortress.layers[0] = {
    .background = "fortressfront",
    .offset = Vec2f(0.f, 0.f),
    .parallax = Vec2f(0.375f, 0.f),
    .repeat_y = false
  };
  fortress.music = "fortress.spc";
  themes["fortress"] = std::move(fortress);

  Theme bonus_room;
  bonus_room.background = Color();
  bonus_room.layers[0] = {
    .background = "bonusquestion",
    .offset = Vec2f(0.f, 0.f),
    .parallax = Vec2f(0.f, 0.f),
    .repeat_y = true
  };
  bonus_room.music = "underworld.spc";
  themes["bonus_room"] = std::move(bonus_room);
}

void BaseGame::exit() {}

void BaseGame::pause() { paused = true; }
void BaseGame::resume() { paused = false; }

void BaseGame::update(float delta) {
  if (not paused) {
    engine->pushState(WorldMap::create());
    pause();
  }
}

void BaseGame::draw(float delta) {}

UInt BaseGame::getCoins() const { return coins; }
UInt BaseGame::getLives() const { return lives; }
ULong BaseGame::getScore() const { return score; }

void BaseGame::addCoins(int count) {
  coins = util::absmod(coins + count, 100);
}

void BaseGame::addLives(int count) {
  lives = std::max(0, std::min(lives + count, 99));
}

void BaseGame::addScore(long count) { score += count; }
void BaseGame::addScore(ULong count) { score += count; }
}
