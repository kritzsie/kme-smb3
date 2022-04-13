#include "world.hpp"

#include "ecs/components.hpp"
#include "../gameplay.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

#include <exception>
#include <type_traits>

#include <cmath>

namespace kme {
// begin Subworld
Subworld::Subworld(const BaseGame* basegame, const Gameplay* gameplay)
: basegame(basegame), gameplay(gameplay) {}

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

float Subworld::getGravity() {
  return gravity;
}

void Subworld::setGravity(float value) {
  gravity = value;
}

// ugly
static Rect<float> toAABB(Vec2f pos, CCollision box) {
  return Rect<float>(pos.x - box.radius, pos.y, box.radius * 2.f, box.height);
}

// ugly
static Rect<int> toRange(Rect<float> aabb) {
  return Rect<int>(
    std::floor(aabb.x), std::floor(aabb.y),
    std::ceil(aabb.width + 1.f), std::ceil(aabb.height + 1.f)
  );
}

void Subworld::update(float delta) {
  // player entity
  if (entities.valid(player)) {
    UInt32& flags = entities.get<CFlags>(player);
    Vec2f& vel = entities.get<CVelocity>(player);
    Direction& direction = entities.get<CDirection>(player);
    EState& state = entities.get<CState>(player);
    auto& timer = entities.get<CTimer>(player);

    float x = 0.f;
    x += gameplay->buttons.at(Gameplay::Action::RIGHT);
    x -= gameplay->buttons.at(Gameplay::Action::LEFT);
    x = std::clamp(x, -1.f, 1.f);

    auto& run = gameplay->buttons.at(Gameplay::Action::RUN);
    auto& jump = gameplay->buttons.at(Gameplay::Action::JUMP);

    if (x != 0) {
      vel.x = std::clamp(vel.x + x * (run ? 32.f : 16.f) * delta, -16.f, 16.f);
      flags |= EFlags::MOVING;
      if      (x < 0) direction = Direction::LEFT;
      else if (x > 0) direction = Direction::RIGHT;
    }
    else {
      flags &= ~EFlags::MOVING;
    }

    if (jump) {
      if (~jump > 0 and ~flags & EFlags::AIRBORNE) {
        timer.jump = 0.25f;
        flags |= EFlags::AIRBORNE;
      }

      if (flags & EFlags::AIRBORNE and timer.jump > 0.f) {
        vel.y = 16.f;
        timer.jump = std::max(timer.jump - 1.f * delta, 0.f);
      }
    }
    else {
      timer.jump = 0.f;
    }

    // TODO: implement p-speed
    float p_meter = 0.f;

    if (flags & EFlags::DEAD) {
      state = EState::DEAD;
    }
    else if (flags & EFlags::DUCKING) {
      state = EState::DUCKING;
    }
    else if (flags & EFlags::AIRBORNE) {
      if (p_meter >= 7.f) {
        state = EState::RUNJUMPING;
      }
      else {
        state = EState::AIRBORNE;
      }
    }
    else if (flags & EFlags::RUNNING) {
      state = EState::RUNNING;
    }
    else if (!(flags & EFlags::AIRBORNE)) {
      if (flags & EFlags::MOVING
      and x * vel.x < 0) {
        state = EState::SLIPPING;
      }
      else if (flags & EFlags::WALKING) {
        state = EState::WALKING;
      }
      else {
        state = EState::IDLE;
      }
    }
  }

  // drawable entities
  auto render_view = entities.view<CInfo, CState, CRender>();
  for (auto entity : render_view) {
    const auto& info = render_view.get<CInfo>(entity);
    const EState& state = render_view.get<CState>(entity);
    auto& rs = render_view.get<CRender>(entity);

    rs.time += delta;

    const auto& states = basegame->entity_data.getRenderStates(info.type);
  }

  // movement code
  auto move_view = entities.view<CFlags, CVelocity>();
  for (auto entity : move_view) {
    UInt32& flags = move_view.get<CFlags>(entity);
    Vec2f& vel = move_view.get<CVelocity>(entity);

    // apply gravity
    float gravity = getGravity();
    if (~flags & EFlags::NOGRAVITY) {
      vel.y += gravity * delta;
    }

    // apply friction
    if (~flags & EFlags::NOFRICTION
    and ~flags & EFlags::AIRBORNE
    and ~flags & EFlags::MOVING) {
      if (vel.x > 0.f) {
        vel.x = std::max(vel.x - 8.f * delta, 0.f);
      }
      else if (vel.x < 0.f) {
        vel.x = std::min(vel.x + 8.f * delta, 0.f);
      }
    }
  }

  // non-collision movement
  auto nocollide_view = entities.view<
    CFlags, CPosition, CVelocity
  >(entt::exclude<CCollision>);
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

    // resolve X axis first to avoid slipping off edges
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
          if (entity_aabb.x > x + 0.5f) {
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
          if (entity_aabb.y > y + 0.5f) {
            landed = true;
            pos.y += collision.height;
          }
          else {
            pos.y -= collision.height;
            timer.jump = 0.f;
          }
          vel.y = 0.f;
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
}
// end Subworld

// begin Level
Level::Level(const BaseGame* basegame, const Gameplay* gameplay)
: basegame(basegame), gameplay(gameplay) {
  createSubworld();
}

std::size_t Level::createSubworld() {
  return createSubworld(counter++);
}

// TODO: de-duplicate this
std::size_t Level::createSubworld(std::size_t index_hint) {
  if (subworld.find(index_hint) == subworld.end()) {
    if (counter == index_hint) {
      ++counter;
    }

    subworld.emplace(index_hint, Subworld(basegame, gameplay));
    return index_hint;
  }

  while (subworld.find(counter) != subworld.end()) {
    ++counter;
  }

  subworld.emplace(counter, Subworld(basegame, gameplay));
  return counter;
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
