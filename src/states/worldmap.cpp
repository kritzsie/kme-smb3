#include "worldmap.hpp"

#include "gameplay.hpp"

namespace kme {
WorldMap::Factory WorldMap::create() {
  return [](BaseState*, Engine* engine) -> BaseState* {
    return new WorldMap(engine);
  };
}

WorldMap::WorldMap(Engine* engine) : BaseState(nullptr, engine) {}

void WorldMap::enter() {}

void WorldMap::exit() {}

void WorldMap::pause() {
  paused = true;
}

void WorldMap::resume() {
  paused = false;
}

void WorldMap::update(float delta) {
  if (not paused) {
    engine->popState();
    engine->pushState(Gameplay::create(1, 1));
    pause();
  }
}

void WorldMap::draw(float delta) {}
}
