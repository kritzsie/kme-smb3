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
Subworld::Subworld(const BaseGame* basegame_new, Gameplay* gameplay_new) {
  basegame = basegame_new;
  gameplay = gameplay_new;
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

void Subworld::setBounds(Rect<int> size_new) {
  bounds = size_new;
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

// begin ugly
static Rect<float> toAABB(Vec2f pos, CCollision box) {
  return Rect<float>(pos.x - box.radius, pos.y, box.radius * 2.f, box.height);
}

static Rect<int> toRange(Rect<float> aabb) {
  return Rect<int>(
    std::floor(aabb.x), std::floor(aabb.y),
    std::ceil(aabb.width + 1.f), std::ceil(aabb.height + 1.f)
  );
}
// end ugly

void Subworld::update(float delta) {
  for (auto entity : entities.view<CInfo>()) {
    if (not entities.get<CInfo>(entity).valid) {
      entities.destroy(entity);
    }
  }

  // player entity
  if (entities.valid(player)) {
    UInt32& flags = entities.get<CFlags>(player);
    Vec2f& vel = entities.get<CVelocity>(player);
    Sign& direction = entities.get<CDirection>(player);
    EState& state = entities.get<CState>(player);
    auto& bbox = entities.get<CCollision>(player);
    auto& timer = entities.get<CTimer>(player);
    auto& render = entities.get<CRender>(player);
    auto& audio = entities.get<CAudio>(player);

    float x = 0.f;
    x += gameplay->inputs.at(Gameplay::Action::RIGHT) > 0.25f;
    x -= gameplay->inputs.at(Gameplay::Action::LEFT)  > 0.25f;
    x = std::clamp(x, -1.f, 1.f);

    const auto& jump_input = gameplay->inputs.at(Gameplay::Action::JUMP);
    bool jump = jump_input > 0.25f;
    bool run  = gameplay->inputs.at(Gameplay::Action::RUN) > 0.25f;
    bool duck = gameplay->inputs.at(Gameplay::Action::DOWN) > 0.25f;

    float max_x = run ? 12.f : 6.f;

    if (~flags & EFlags::AIRBORNE) {
      flags &= ~EFlags::DUCKING;
      bbox.height = 25.f / 16.f;
    }

    if (not jump) {
      flags &= ~EFlags::NOGRAVITY;
    }

    if (x != 0) {
      flags |= EFlags::NOFRICTION;
      direction = toSign(x);
      if (direction * vel.x < 0) {
        vel.x += direction * 16.f * delta;
      }
      vel.x = vel.x + direction * 16.f * delta;
      vel.x = std::clamp(vel.x, -max_x, max_x);
    }
    else {
      flags &= ~EFlags::NOFRICTION;
      if (duck and ~flags & EFlags::AIRBORNE) {
        flags |= EFlags::DUCKING;
        bbox.height = 15.f / 16.f;
      }
    }

    if (jump) {
      if (~jump_input > 0 and ~flags & EFlags::AIRBORNE) {
        flags |= EFlags::AIRBORNE;
        timer.jump = 0.275f + std::min(std::abs(vel.x) / 12.f / 10.f, 0.125f);
        gameplay->playSound("jump");
      }

      if (flags & EFlags::AIRBORNE and timer.jump > 0.f) {
        flags |= EFlags::NOGRAVITY;
        vel.y = 12.f;
        timer.jump = std::max(timer.jump - 1.f * delta, 0.f);
      }
      else {
        flags &= ~EFlags::NOGRAVITY;
      }
    }
    else {
      timer.jump = 0.f;
    }

    // TODO: implement p-speed
    float p_meter = 0.f;

    EState state_prev = state;
    if (flags & EFlags::DEAD) {
      state = EState::DEAD;
    }
    else if (flags & EFlags::DUCKING) {
      state = EState::DUCK;
    }
    else if (flags & EFlags::AIRBORNE) {
      if (p_meter < 7.f) {
        state = EState::AIRBORNE;
      }
      else {
        state = EState::RUNJUMP;
      }
    }
    else if (flags & EFlags::RUNNING) {
      state = EState::RUN;
    }
    else if (~flags & EFlags::AIRBORNE) {
      if (x != 0 and direction * vel.x < 0) {
        state = EState::SLIP;
        if (audio.channels.slip == Sound::MAX_VOICES) {
          audio.channels.slip = gameplay->playSoundLoop("slip");
        }
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

      if (state_prev == EState::SLIP) {
        gameplay->stopSoundLoop(audio.channels.slip);
        audio.channels.slip = Sound::MAX_VOICES;
      }
    }
  }

  // movement code
  auto move_view = entities.view<CFlags, CVelocity>();
  for (auto entity : move_view) {
    UInt32& flags = move_view.get<CFlags>(entity);
    Vec2f& vel = move_view.get<CVelocity>(entity);

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
        vel.x = std::max(vel.x - 12.f * delta, 0.f);
      }
      else if (vel.x < 0.f) {
        vel.x = std::min(vel.x + 12.f * delta, 0.f);
      }
    }
  }

  // non-collision movement
  auto nocollide_view = entities.view<CFlags, CPosition, CVelocity>(entt::exclude<CCollision>);
  for (auto entity : nocollide_view) {
    UInt32& flags = nocollide_view.get<CFlags>(entity);
    Vec2f& pos = nocollide_view.get<CPosition>(entity);
    Vec2f& vel = nocollide_view.get<CVelocity>(entity);

    flags |= EFlags::AIRBORNE;
    pos += vel * delta;
  }

  // collision code
  auto collide_view = entities.view<
    CFlags, CTimer,
    CPosition, CVelocity, CCollision
  >();
  for (auto entity : collide_view) {
    UInt32& flags = collide_view.get<CFlags>(entity);
    auto& timer = collide_view.get<CTimer>(entity);
    Vec2f& pos = collide_view.get<CPosition>(entity);
    Vec2f& vel = collide_view.get<CVelocity>(entity);
    auto& bbox = collide_view.get<CCollision>(entity);

    Rect<float> entity_aabb;
    Rect<int> range;

    bool landed = false;

    pos.y += vel.y * delta;

    entity_aabb = toAABB(pos, bbox);
    range = toRange(entity_aabb);
    for (int y = range.y; y < range.y + range.height; ++y)
    for (int x = range.x; x < range.x + range.width;  ++x) {
      Rect<float> tile_aabb = Rect<float>(x, y, 1.f, 1.f);
      if (entity_aabb.intersects(tile_aabb)) {
        switch (basegame->level_tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
        case TileDef::CollisionType::SOLID: {
          Rect<float> collision = entity_aabb.intersection(tile_aabb);
          if (entity_aabb.y >= y + 0.5f) {
            landed = true;
            pos.y += collision.height;
          }
          else {
            pos.y -= collision.height;
            timer.jump = 0.f;
            gameplay->playSound("bump");
          }
          vel.y = 0.f;
          entity_aabb = toAABB(pos, bbox);
          break;
        }
        case TileDef::CollisionType::PLATFORM: {
          Rect<float> collision = entity_aabb.intersection(tile_aabb);
          if (entity_aabb.y > y + 0.5f
          and pos.y - vel.y * delta >= y + 14.f / 16.f) {
            landed = true;
            pos.y += collision.height;
            vel.y = 0.f;
            entity_aabb = toAABB(pos, bbox);
          }
          break;
        }
        case TileDef::CollisionType::NONE:
        default:
          break;
        }
      }
    }

    pos.x += vel.x * delta;

    entity_aabb = toAABB(pos, bbox);
    range = toRange(entity_aabb);
    for (int y = range.y; y < range.y + range.height; ++y)
    for (int x = range.x; x < range.x + range.width;  ++x) {
      Rect<float> tile_aabb = Rect<float>(x, y, 1.f, 1.f);
      if (entity_aabb.intersects(tile_aabb)) {
        switch (basegame->level_tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
        case TileDef::CollisionType::SOLID: {
          Rect<float> collision = entity_aabb.intersection(tile_aabb);
          if (entity_aabb.x >= x + 0.5f) {
            pos.x += collision.width;
          }
          else {
            pos.x -= collision.width;
          }
          vel.x = 0.f;
          entity_aabb = toAABB(pos, bbox);
          break;
        }
        case TileDef::CollisionType::NONE:
        default:
          break;
        }
      }
    }

    if (landed) flags &= ~EFlags::AIRBORNE;
    else        flags |=  EFlags::AIRBORNE;
  }

  if (entities.valid(camera)) {
    auto& info = entities.get<CInfo>(camera);

    if (entities.valid(info.parent)) {
      Vec2f& parent_pos = entities.get<CPosition>(info.parent);
      Vec2f& pos = entities.get<CPosition>(camera);
      auto& bbox = entities.get<CCollision>(camera);

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
        bbox.radius + bounds.x,
        bounds.width - bbox.radius + bounds.x
      );
      pos.y = std::clamp(
        pos.y,
        0.f + bounds.y,
        bounds.height - bbox.height / 2 + bounds.y
      );
    }

    if (entities.valid(player)) {
      Vec2f& player_pos = entities.get<CPosition>(player);
      Vec2f& player_vel = entities.get<CVelocity>(player);
      auto& player_bbox = entities.get<CCollision>(player);
      Vec2f& pos = entities.get<CPosition>(camera);
      auto& bbox = entities.get<CCollision>(camera);

      if (player_pos.x < pos.x - bbox.radius + player_bbox.radius) {
        player_pos.x = pos.x - bbox.radius + player_bbox.radius;
        player_vel.x = 0.f;
      }
      else if (player_pos.x > pos.x + bbox.radius - player_bbox.radius) {
        player_pos.x = pos.x + bbox.radius - player_bbox.radius;
        player_vel.x = 0.f;
      }
    }
  }

  // drawable entities
  auto render_view = entities.view<CInfo, CVelocity, CState, CRender>();
  for (auto entity : render_view) {
    auto& info = render_view.get<CInfo>(entity);
    Vec2f& vel = render_view.get<CVelocity>(entity);
    const EState& state = render_view.get<CState>(entity);
    auto& render = render_view.get<CRender>(entity);

    auto& states = basegame->entity_data.getRenderStates(info.type);

    switch (state) {
    case EState::AIRBORNE:
      render.state.setState("JUMP", states->getFrameOffset("JUMP", render.time));
      break;
    case EState::DUCK:
      render.state.setState("DUCK", states->getFrameOffset("DUCK", render.time));
      break;
    case EState::SLIP:
      render.state.setState("SLIP", states->getFrameOffset("SLIP", render.time));
      break;
    case EState::WALK:
      render.state.setState("WALK", states->getFrameOffset("WALK", render.time));
      break;
    case EState::DEAD:
      render.state.setState("DEAD", states->getFrameOffset("DEAD", render.time));
      break;
    case EState::IDLE:
    default:
      render.state.setState("IDLE", states->getFrameOffset("IDLE", render.time));
      break;
    }

    render.time += std::clamp(std::abs(vel.x) / 4.f, 1.f, 4.f) * delta;
  }
}
// end Subworld

// begin Level
Level::Level(const BaseGame* basegame_new, Gameplay* gameplay_new) {
  basegame = basegame_new;
  gameplay = gameplay_new;
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
