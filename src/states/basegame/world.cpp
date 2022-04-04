#include "world.hpp"

#include "entity.hpp"

#include "ecs/components.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

#include <exception>
#include <type_traits>

#include <cmath>

namespace kme {
// begin Subworld
Subworld::Subworld(const TileDefs& tile_data)
: tile_data(tile_data) {}

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
static Rect<float> toAABB(Vec2f pos, CollisionComponent box) {
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
  // player input
  if (entities.valid(player)) {
    UInt32& flags = entities.get<FlagsComponent>(player);
    auto& timer = entities.get<TimerComponent>(player);
    Vec2f& vel = entities.get<VelocityComponent>(player);

    float x = 0.f;
    x += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) / 100.f;
    x += sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    x -= sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    x = std::clamp(x, -1.f, 1.f);

    bool run = false;
    run |= sf::Joystick::isButtonPressed(0, 2);
    run |= sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z);

    if (x != 0) {
      vel.x = std::clamp(vel.x + x * (run ? 32.f : 16.f) * delta, -16.f, 16.f);
      flags |= FlagsComponent::NOFRICTION;
    }
    else {
      flags &= ~FlagsComponent::NOFRICTION;
    }

    bool jump = false;
    jump |= sf::Joystick::isButtonPressed(0, 0);
    jump |= sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);

    if (jump) {
      if (~flags & FlagsComponent::AIRBORNE) {
        timer.jump = 0.25f;
        flags |= FlagsComponent::AIRBORNE;
      }

      if (flags & FlagsComponent::AIRBORNE and timer.jump > 0.f) {
        vel.y = 16.f;
        timer.jump = std::max(timer.jump - 1.f * delta, 0.f);
      }
    }
    else {
      timer.jump = 0.f;
    }
  }

  // movement code
  auto move_view = entities.view<FlagsComponent, VelocityComponent>();
  for (entt::entity entity : move_view) {
    UInt32& flags = move_view.get<FlagsComponent>(entity);
    Vec2f& vel = move_view.get<VelocityComponent>(entity);

    // apply gravity
    float gravity = getGravity();
    if (~flags & FlagsComponent::NOGRAVITY) {
      vel.y += gravity * delta;
    }

    // apply friction
    if (~flags & FlagsComponent::NOFRICTION
    and ~flags & FlagsComponent::AIRBORNE) {
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
    FlagsComponent, PositionComponent, VelocityComponent
  >(entt::exclude<CollisionComponent>);
  for (entt::entity entity : nocollide_view) {
    UInt32& flags = nocollide_view.get<FlagsComponent>(entity);
    Vec2f& pos = nocollide_view.get<PositionComponent>(entity);
    Vec2f& vel = nocollide_view.get<VelocityComponent>(entity);

    flags |= FlagsComponent::AIRBORNE;
    pos += vel * delta;
  }

  // collision code
  auto collide_view = entities.view<
    FlagsComponent, TimerComponent,
    PositionComponent, VelocityComponent, CollisionComponent
  >();
  for (entt::entity entity : collide_view) {
    UInt32& flags = collide_view.get<FlagsComponent>(entity);
    auto& timer = collide_view.get<TimerComponent>(entity);
    Vec2f& pos = collide_view.get<PositionComponent>(entity);
    Vec2f& vel = collide_view.get<VelocityComponent>(entity);
    auto& bbox = collide_view.get<CollisionComponent>(entity);

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
        switch (tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
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
        switch (tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
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

    if (landed) flags &= ~FlagsComponent::AIRBORNE;
    else        flags |=  FlagsComponent::AIRBORNE;
  }

  /*
  // move player
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    Entity entity = getEntity(*iter);

    if (entity.get<Type>() == "player_mario") {
      Vec2f vel = entity.get<Velocity>();
      UInt32 flags = entity.get<Flags>();

      float x = 0.f;
      x += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) / 100.f;
      x += sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
      x -= sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
      x = std::clamp(x, -1.f, 1.f);

      bool jump = false;
      jump |= sf::Joystick::isButtonPressed(0, 1);
      jump |= sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);

      if (x) {
        vel.x = std::clamp(vel.x + x, -8.f, 8.f);
      }

      if (jump) {
        if (flags & Flags::LANDED) {
          vel.y = 12.f;
        }
      }

      entity.set<Velocity>(vel);
    }
  }

  commit_entities();

  // apply friction
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    Entity entity = getEntity(*iter);

    Vec2f vel = entity.get<Velocity>();
    UInt32 flags = entity.get<Flags>();

    if (flags & Flags::LANDED) {
      if (vel.x > 0.f) {
        vel.x = std::max(vel.x - 8.f * delta, 0.f);
      }
      else if (vel.x < 0.f) {
        vel.x = std::min(vel.x + 8.f * delta, 0.f);
      }

      entity.set<Velocity>(vel);
    }
  }

  commit_entities();

  // apply gravity
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    Entity entity = getEntity(*iter);

    if (entity.get<Flags>() & Flags::GRAVITY) {
      Vec2f vel = entity.get<Velocity>();
      vel.y -= 29.625f * delta;
      entity.set<Velocity>(vel);
    }
  }

  commit_entities();

  // move and collide
  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    Entity entity = getEntity(*iter);
    Vec2f pos = entity.get<Position>();
    Vec2f vel = entity.get<Velocity>();
    bool landed = false;

    pos.x += vel.x * delta;

    Rect<float> entity_aabb = toAABB(pos, entity.get<CollisionBox>());
    Rect<int> range = toRange(entity_aabb);
    for (int y = range.y; y < range.y + range.height; ++y)
    for (int x = range.x; x < range.x + range.width;  ++x) {
      Rect<float> tile_aabb = Rect<float>(x, y, 1.f, 1.f);
      if (entity_aabb.intersects(tile_aabb)) {
        switch (tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
        case TileDef::CollisionType::SOLID: {
          Rect<float> collision = entity_aabb.intersection(tile_aabb);
          if (entity_aabb.x > x + 0.5f) {
            pos.x += collision.width;
          }
          else {
            pos.x -= collision.width;
          }
          vel.x = 0.f;
          entity_aabb = toAABB(pos, entity.get<CollisionBox>());
          break;
        }
        case TileDef::CollisionType::NONE:
        default:
          break;
        }
      }
    }

    pos.y += vel.y * delta;

    entity_aabb = toAABB(pos, entity.get<CollisionBox>());
    range = toRange(entity_aabb);
    for (int y = range.y; y < range.y + range.height; ++y)
    for (int x = range.x; x < range.x + range.width;  ++x) {
      Rect<float> tile_aabb = Rect<float>(x, y, 1.f, 1.f);
      if (entity_aabb.intersects(tile_aabb)) {
        switch (tile_data.getTileDef(tiles[x][y]).getCollisionType()) {
        case TileDef::CollisionType::SOLID: {
          Rect<float> collision = entity_aabb.intersection(tile_aabb);
          if (entity_aabb.y > y + 0.5f) {
            landed = true;
            pos.y += collision.height;
          }
          else {
            pos.y -= collision.height;
          }
          vel.y = 0.f;
          entity_aabb = toAABB(pos, entity.get<CollisionBox>());
          break;
        }
        case TileDef::CollisionType::NONE:
        default:
          break;
        }
      }
    }

    entity.set<Position>(pos);
    entity.set<Velocity>(vel);

    if (landed) entity.set<Flags>(entity.get<Flags>() |  Flags::LANDED);
    else        entity.set<Flags>(entity.get<Flags>() & ~Flags::LANDED);
  }

  commit_entities();
  */
}
// end Subworld

// begin Level
Level::Level(const TileDefs& tile_data)
: tile_data(tile_data) {
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

    subworld.emplace(index_hint, Subworld(tile_data));
    return index_hint;
  }

  while (subworld.find(counter) != subworld.end()) {
    ++counter;
  }

  subworld.emplace(counter, Subworld(tile_data));
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
