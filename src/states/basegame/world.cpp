#include "world.hpp"

#include "ecs/components.hpp"
#include "../gameplay.hpp"
#include "../../sound.hpp"
#include "../../util.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

#include <exception>
#include <type_traits>

#include <cmath>

namespace kme {
// begin Subworld
Subworld::Subworld(const BaseGame* basegame_arg, Gameplay* gameplay_arg) {
  basegame = basegame_arg;
  gameplay = gameplay_arg;
}

const EntityRegistry& Subworld::getEntities() const {
  return entities;
}

EntityRegistry& Subworld::getEntities() {
  return const_cast<EntityRegistry&>(static_cast<const Subworld*>(this)->getEntities());
}

const Tilemap& Subworld::getTiles() const {
  return tiles;
}

Tilemap& Subworld::getTiles() {
  return const_cast<Tilemap&>(static_cast<const Subworld*>(this)->getTiles());
}

Rect<int> Subworld::getBounds() const {
  return bounds;
}

void Subworld::setBounds(Rect<int> size_arg) {
  bounds = size_arg;
}

void Subworld::setBounds(int x, int y, int width, int height) {
  setBounds(Rect<int>(x, y, width, height));
}

void Subworld::setBounds(int width, int height) {
  setBounds(0, 0, width, height);
}

float Subworld::getGravity() const {
  return gravity;
}

void Subworld::setGravity(float value) {
  gravity = value;
}

std::string Subworld::getTheme() const {
  return theme;
}

void Subworld::setTheme(std::string theme_arg) {
  theme = theme_arg;
  auto theme_ptr = basegame->themes.at(theme);
  gameplay->playMusic(theme_ptr->music);
}

// begin ugly
static Rect<int> toRange(Rect<float> aabb) {
  return Rect<int>(
    std::floor(aabb.x), std::floor(aabb.y),
    std::ceil(aabb.width + 1.f), std::ceil(aabb.height + 1.f)
  );
}
// end ugly

void Subworld::update(float delta) {
  auto info_view = entities.view<CInfo>();
  for (auto entity : info_view) {
    if (not info_view.get<CInfo>(entity).valid) {
      entities.destroy(entity);
    }
  }

  auto collision_view = entities.view<CPosition, CCollision>();
  for (auto entity : collision_view) {
    auto& pos = collision_view.get<CPosition>(entity).value;
    auto& coll = collision_view.get<CCollision>(entity);
    coll.pos_old = pos;
  }

  // update render timer
  auto render_view = entities.view<CRender>();
  for (auto entity : render_view) {
    auto& render = render_view.get<CRender>(entity);

    bool animate_faster = false;

    auto vel_view = entities.view<CVelocity, CState>();
    if (vel_view.contains(entity)) {
      auto& state = vel_view.get<CState>(entity).value;
      if (state == EState::WALK) {
        animate_faster = true;
      }
    }

    if (animate_faster) {
      auto& vel = vel_view.get<CVelocity>(entity).value;
      render.time += std::clamp(std::abs(vel.x) / 3.f, 1.f, 4.f) * delta;
    }
    else {
      render.time += delta;
    }
  }

  // player entity
  if (entities.valid(player)) {
    auto& info = entities.get<CInfo>(player);
    auto& flags = entities.get<CFlags>(player).value;
    auto& vel = entities.get<CVelocity>(player).value;
    auto& direction = entities.get<CDirection>(player).value;
    auto& state = entities.get<CState>(player).value;
    auto& powerup = entities.get<CPowerup>(player).value;
    auto& coll = entities.get<CCollision>(player);
    auto& counters = entities.get<CCounters>(player);
    auto& timers = entities.get<CTimers>(player);
    auto& render = entities.get<CRender>(player);
    auto& audio = entities.get<CAudio>(player);

    auto hitboxes = basegame->entity_data.getHitboxes(info.type);
    auto hitbox_states = hitboxes->at(powerup);

    float x = 0.f;
    x += gameplay->inputs.at(Gameplay::Action::RIGHT) > 0.25f;
    x -= gameplay->inputs.at(Gameplay::Action::LEFT)  > 0.25f;
    x = std::clamp(x, -1.f, 1.f);

    const auto& jump_input = gameplay->inputs.at(Gameplay::Action::JUMP);
    bool jump = jump_input > 0.25f;
    bool run  = gameplay->inputs.at(Gameplay::Action::RUN) > 0.25f;
    bool duck = gameplay->inputs.at(Gameplay::Action::DOWN) > 0.25f;

    float max_x = run ? (flags & EFlags::RUNNING ? 12.f : 10.f) : 5.f;

    if (x != 0) {
      direction = toSign(x);
    }

    if (flags & EFlags::AIRBORNE) {
      flags |= EFlags::NOFRICTION;
      if (x > 0) {
        if (vel.x <= max_x) {
          vel.x += (vel.x < 0 ? 30.f : 15.f) * delta;
          vel.x = std::min(vel.x, max_x);
        }
      }
      else if (x < 0) {
        if (vel.x >= -max_x) {
          vel.x -= (vel.x > 0 ? 30.f : 15.f) * delta;
          vel.x = std::max(vel.x, -max_x);
        }
      }
    }
    else if (x != 0) {
      if (x > 0) {
        if (vel.x <= max_x) {
          flags |= EFlags::NOFRICTION;
          vel.x += (vel.x < 0 ? 30.f : 15.f) * delta;
          vel.x = std::min(vel.x, max_x);
        }
      }
      else if (x < 0) {
        if (vel.x >= -max_x) {
          flags |= EFlags::NOFRICTION;
          vel.x -= (vel.x > 0 ? 30.f : 15.f) * delta;
          vel.x = std::max(vel.x, -max_x);
        }
      }
    }

    if (~flags & EFlags::AIRBORNE) {
      if (x == 0 or std::abs(vel.x) > max_x) {
        flags &= ~EFlags::NOFRICTION;
      }

      if (duck and x == 0 and hitbox_states.find(EState::DUCK) != hitbox_states.end()) {
        flags |= EFlags::DUCKING;
      }
      else {
        flags &= ~EFlags::DUCKING;
      }
    }

    if (timers.p_speed > 0.f) {
      bool reset_p_meter = false;

      if (powerup == Powerup::LEAF
      or  powerup == Powerup::TANUKI) {
        timers.p_speed = std::max(timers.p_speed - delta, 0.f);
        reset_p_meter = timers.p_speed == 0.f;
      }
      else {
        if (~flags & EFlags::LANDED) {
          timers.p_speed = std::max(timers.p_speed - delta, 0.f);
          reset_p_meter = timers.p_speed == 0.f;
        }
        else {
          timers.p_speed = 0.f;
        }
      }

      if (reset_p_meter) {
        counters.p_meter = 0.f;
      }
    }

    if (timers.p_speed == 0.f) {
      if (flags & EFlags::LANDED and counters.p_meter > 6.0f and std::abs(vel.x) < 5.f) {
        counters.p_meter = std::min(counters.p_meter - 0.5f, 7.f);
      }
      else if (~flags & EFlags::AIRBORNE and x != 0 and std::abs(vel.x) >= 10.f) {
        counters.p_meter = std::min(counters.p_meter + 6.f * delta, 7.f);
      }
      else if (counters.p_meter > 0.f) {
        if (~flags & EFlags::AIRBORNE
        or  ~flags & EFlags::RUNNING) {
          counters.p_meter = std::max(counters.p_meter - 2.5f * delta, 0.f);
        }
      }
    }

    if (counters.p_meter > 6.f) {
      flags |= EFlags::RUNNING;
      if (audio.channels.speed == Sound::MAX_VOICES) {
        audio.channels.speed = gameplay->playSoundLoop("running");
      }
    }
    else {
      flags &= ~EFlags::RUNNING;
      if (audio.channels.speed != Sound::MAX_VOICES) {
        gameplay->stopSoundLoop(audio.channels.speed);
        audio.channels.speed = Sound::MAX_VOICES;
      }
    }

    if (jump) {
      if (~jump_input > 0 and ~flags & EFlags::AIRBORNE) {
        timers.jump = 0.275f + std::min(std::abs(vel.x) / 10.f / 10.f, 0.125f);
        if (counters.p_meter > 6.f and timers.p_speed == 0.f) {
          if (std::abs(vel.x) >= 10.f) {
            counters.p_meter = 7.f;
          }
          timers.p_speed = 4.f;
        }
        gameplay->playSound("jump");
      }

      if (timers.jump > 0.f) {
        flags |= EFlags::NOGRAVITY;
        vel.y = 12.f;
        timers.jump = std::max(timers.jump - 1.f * delta, 0.f);
      }
      else {
        flags &= ~EFlags::NOGRAVITY;
      }
    }
    else {
      flags &= ~EFlags::NOGRAVITY;
      timers.jump = 0.f;
    }

    EState state_prev = state;
    if (flags & EFlags::DEAD) {
      state = EState::DEAD;
    }
    else if (flags & EFlags::DUCKING) {
      state = EState::DUCK;
    }
    else if (flags & EFlags::AIRBORNE) {
      if (~flags & EFlags::RUNNING) {
        state = EState::AIRBORNE;
      }
      else {
        state = EState::RUNJUMP;
      }
    }
    else if (~flags & EFlags::AIRBORNE) {
      if (x != 0 and direction * vel.x < 0) {
        state = EState::SLIP;
        if (audio.channels.slip == Sound::MAX_VOICES) {
          audio.channels.slip = gameplay->playSoundLoop("slip");
        }
      }
      else if (std::abs(vel.x) >= 11.f) {
        state = EState::RUN;
      }
      else if (vel.x != 0) {
        state = EState::WALK;
      }
      else {
        state = EState::IDLE;
      }
    }

    if (state != state_prev) {
      render.time = 0.f;

      if (hitbox_states.find(state) != hitbox_states.end()) {
        coll.hitbox = hitbox_states[state];
      }
      else {
        coll.hitbox = hitbox_states[EState::IDLE];
      }

      if (state_prev == EState::SLIP) {
        gameplay->stopSoundLoop(audio.channels.slip);
        audio.channels.slip = Sound::MAX_VOICES;
      }
    }
  }

  //auto collision_view = entities.view<CCollision>();
  for (auto entity : collision_view) {
    auto& coll = collision_view.get<CCollision>(entity);
    coll.tiles.clear();
  }

  // movement code
  auto gravity_and_friction_view = entities.view<CFlags, CVelocity>();
  auto move_view = entities.view<CPosition>();
  auto collide_view = entities.view<CCollision>();
  for (auto entity : gravity_and_friction_view) {
    auto& flags = gravity_and_friction_view.get<CFlags>(entity).value;
    auto& vel = gravity_and_friction_view.get<CVelocity>(entity).value;

    // apply gravity
    float gravity = getGravity();
    if (~flags & EFlags::NOGRAVITY) {
      float min_y = -15.f;

      vel.y += gravity * delta;
      vel.y = std::max(vel.y, min_y);
    }

    // apply friction
    if (~flags & EFlags::NOFRICTION
    and ~flags & EFlags::AIRBORNE) {
      if (vel.x > 0.f) {
        vel.x = std::max(vel.x - 10.f * delta, 0.f);
      }
      else if (vel.x < 0.f) {
        vel.x = std::min(vel.x + 10.f * delta, 0.f);
      }
    }

    if (move_view.contains(entity)) {
      auto& pos = move_view.get<CPosition>(entity).value;

      if (flags & EFlags::NOCLIP or not collide_view.contains(entity)) {
        pos += vel * delta;
      }
      else {
        Vec2f pos_old = pos;
        pos.x = pos_old.x + vel.x * delta;
        checkWorldCollisions(entity);
        pos.y = pos_old.y + vel.y * delta;
        checkWorldCollisions(entity);
        pos = pos_old + vel * delta;
        checkWorldCollisions(entity);

        resolveWorldCollisions(entity);
      }
    }
  }

  if (entities.valid(camera)) {
    auto& info = entities.get<CInfo>(camera);

    if (entities.valid(info.parent)) {
      auto& parent_pos = entities.get<CPosition>(info.parent).value;
      auto& pos = entities.get<CPosition>(camera).value;
      auto& coll = entities.get<CCollision>(camera);

      // position camera relative to world
      if (parent_pos.x - 1.f > pos.x) {
        pos.x = parent_pos.x - 1.f;
      }
      else if (parent_pos.x + 1.f < pos.x) {
        pos.x = parent_pos.x + 1.f;
      }

      if (parent_pos.y - 1.5f > pos.y) {
        pos.y = parent_pos.y - 1.5f;
      }
      else if (parent_pos.y + 3.f < pos.y) {
        pos.y = parent_pos.y + 3.f;
      }

      // restrict camera to world boundaries
      pos.x = std::clamp(
        pos.x,
        coll.hitbox.radius + bounds.x,
        bounds.width - coll.hitbox.radius + bounds.x
      );
      pos.y = std::clamp(
        pos.y,
        0.f + bounds.y,
        bounds.height - coll.hitbox.height / 2 + bounds.y
      );
    }

    if (entities.valid(player)) {
      auto& player_pos = entities.get<CPosition>(player).value;
      auto& player_vel = entities.get<CVelocity>(player).value;
      auto& player_coll = entities.get<CCollision>(player);
      auto& pos = entities.get<CPosition>(camera).value;
      auto& coll = entities.get<CCollision>(camera);

      if (player_pos.x < pos.x - coll.hitbox.radius + player_coll.hitbox.radius + 0.25f) {
        player_pos.x = pos.x - coll.hitbox.radius + player_coll.hitbox.radius + 0.25f;
        player_vel.x = 0.f;
      }
      else if (player_pos.x > pos.x + coll.hitbox.radius - player_coll.hitbox.radius - 0.25f) {
        player_pos.x = pos.x + coll.hitbox.radius - player_coll.hitbox.radius - 0.25f;
        player_vel.x = 0.f;
      }
    }
  }

  // drawable entities
  auto renderstate_view = entities.view<CInfo, CState, CRender>();
  auto powerup_view = entities.view<CPowerup>();
  for (auto entity : renderstate_view) {
    auto& info = renderstate_view.get<CInfo>(entity);
    auto& state = renderstate_view.get<CState>(entity).value;
    auto& render = renderstate_view.get<CRender>(entity);

    auto states = basegame->entity_data.getRenderStates(info.type);

    std::string label = getStateName(state).data();
    if (powerup_view.contains(entity)) {
      auto& powerup = powerup_view.get<CPowerup>(entity).value;
      if (powerup != Powerup::NONE) {
        label = util::join({label, getPowerupName(powerup).data()}, ".");
      }
    }

    render.state.setState(label, states->getFrameOffset(label, render.time));
  }

  consumeEvents();
}
// end Subworld

void Subworld::genEvent(EventType type, Event event) {
  switch (type) {
  case EventType::COLLISION: {
    auto coll_event = std::get<CollisionEvent>(event);
    switch (coll_event.type) {
    case CollisionEvent::Type::WORLD: {
      auto world_coll = std::get<WorldCollision>(coll_event.collision);
      auto& coll = entities.get<CCollision>(world_coll.entity);
      coll.tiles.insert(world_coll.tile);
      world_collisions.insert(world_coll);
      break;
    }
    case CollisionEvent::Type::ENTITY:
      auto ent_coll = std::get<EntityCollision>(coll_event.collision);
      entity_collisions.insert(ent_coll);
      break;
    }
    break;
  }
  }
}

void Subworld::genCollisionEvent(Entity entity, Vec2i tile) {
  genEvent(EventType::COLLISION, CollisionEvent {
    .type = CollisionEvent::Type::WORLD,
    .collision = WorldCollision {
      .entity = entity,
      .tile = tile
    }
  });
}

void Subworld::consumeEvents() {
  for (const auto& event : world_collisions) {
  }

  for (const auto& event : entity_collisions) {
  }

  world_collisions.clear();
  entity_collisions.clear();
}

void Subworld::checkWorldCollisions(Entity entity) {
  Vec2f& pos = entities.get<CPosition>(entity).value;
  auto& coll = entities.get<CCollision>(entity);

  Rect<float> ent_aabb = coll.hitbox.toAABB(pos);
  Rect<int> range = toRange(ent_aabb);
  for (int y = range.y; y < range.y + range.height; ++y)
  for (int x = range.x; x < range.x + range.width;  ++x) {
    Rect<float> tile_aabb = Rect<float>(x, y, 1.f, 1.f);
    switch (basegame->level_tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
    case TileDef::CollisionType::SOLID: {
      if (ent_aabb.intersects(tile_aabb)) {
        genCollisionEvent(entity, Vec2(x, y));
      }
      break;
    }
    case TileDef::CollisionType::PLATFORM: {
      if (coll.pos_old.y >= y + 0.5f) {
        genCollisionEvent(entity, Vec2(x, y));
      }
      break;
    }
    default:
      break;
    }
  }
}

void Subworld::resolveWorldCollisions(Entity entity) {
  auto& flags = entities.get<CFlags>(entity).value;
  auto& pos = entities.get<CPosition>(entity).value;
  auto& vel = entities.get<CVelocity>(entity).value;
  auto& coll = entities.get<CCollision>(entity);

  Vec2f best_move;

  for (const auto& i : coll.tiles) {
    Vec2f pos_new = Vec2f(pos.x, coll.pos_old.y);
    Tile tile = tiles[i.x][i.y];
    TileDef tile_data = basegame->level_tile_data.getTileDef(tile);
    Rect<float> ent_aabb = coll.hitbox.toAABB(pos_new);
    Rect<float> tile_aabb = Rect<float>(i.x, i.y, 1.f, 1.f);
    if (ent_aabb.intersects(tile_aabb)) {
      auto collision = ent_aabb.intersection(tile_aabb);
      switch (tile_data.getCollisionType()) {
      case TileDef::CollisionType::SOLID:
        if (coll.pos_old.x >= pos_new.x)
          best_move.x = collision.width;
        else
          best_move.x = -collision.width;
        break;
      case TileDef::CollisionType::SLOPE:
        break;
      default:
        break;
      }
    }
  }

  for (const auto& i : coll.tiles) {
    Vec2f pos_new = Vec2f(pos.x + best_move.x, pos.y);
    Tile tile = tiles[i.x][i.y];
    TileDef tiledef = basegame->level_tile_data.getTileDef(tile);
    Rect<float> ent_aabb = coll.hitbox.toAABB(pos_new);
    Rect<float> tile_aabb = Rect<float>(i.x, i.y, 1.f, 1.f);
    if (ent_aabb.intersects(tile_aabb)) {
      auto collision = ent_aabb.intersection(tile_aabb);
      switch (tiledef.getCollisionType()) {
      case TileDef::CollisionType::SOLID:
        if (coll.pos_old.y >= pos_new.y)
          best_move.y = collision.height;
        else
          best_move.y = -collision.height;
        break;
      case TileDef::CollisionType::PLATFORM:
        if (coll.pos_old.y >= pos_new.y)
          best_move.y = collision.height;
        break;
      case TileDef::CollisionType::SLOPE:
        break;
      default:
        break;
      }
    }
  }

  pos += best_move;

  auto timers_view = entities.view<CTimers>();

  if (best_move.x > 0.f) {
    vel.x = 0.f;
  }
  else if (best_move.x < 0.f) {
    vel.x = 0.f;
  }

  if (best_move.y > 0.f) {
    if (flags & EFlags::AIRBORNE) {
      flags |= EFlags::LANDED;
    }
    else {
      flags &= ~EFlags::LANDED;
    }
    flags &= ~EFlags::AIRBORNE;
    vel.y = 0.f;
  }
  else {
    flags |= EFlags::AIRBORNE;
    flags &= ~EFlags::LANDED;

    if (best_move.y < 0.f) {
      if (timers_view.contains(entity)) {
        auto& timers = timers_view.get<CTimers>(entity);
        timers.jump = 0.f;
      }
      if (vel.y > 0.f) {
        gameplay->playSound("bump");
      }
      vel.y = 0.f;
    }
  }
}

// begin Level
Level::Level(const BaseGame* basegame_arg, Gameplay* gameplay_arg) {
  basegame = basegame_arg;
  gameplay = gameplay_arg;
  createSubworld();
}

std::size_t Level::createSubworld() {
  return createSubworld(count++);
}

// TODO: de-duplicate this
std::size_t Level::createSubworld(std::size_t index_hint) {
  if (subworld.find(index_hint) == subworld.end()) {
    if (count == index_hint) {
      ++count;
    }

    subworld.emplace(index_hint, Subworld(basegame, gameplay));
    return index_hint;
  }

  while (subworld.find(count) != subworld.end()) {
    ++count;
  }

  subworld.emplace(count, Subworld(basegame, gameplay));
  return count;
}

bool Level::subworldExists(std::size_t index) {
  return index == 0 or subworld.find(index) != subworld.end();
}

bool Level::deleteSubworld(std::size_t index) {
  if (subworldExists(index)) {
    subworld.erase(index);
    return true;
  }

  return false;
}

const Subworld& Level::getSubworld(std::size_t index) const {
  return subworld.at(index);
}

Subworld& Level::getSubworld(std::size_t index) {
  return const_cast<Subworld&>(static_cast<const Level*>(this)->getSubworld(index));
}

const Level::const_iterator Level::begin() const {
  return subworld.cbegin();
}

const Level::const_iterator Level::end() const {
  return subworld.cend();
}

const Level::const_iterator Level::cbegin() const {
  return begin();
}

const Level::const_iterator Level::cend() const {
  return end();
}

Level::iterator Level::begin() {
  return subworld.begin();
}

Level::iterator Level::end() {
  return subworld.end();
}
// end Level
}
