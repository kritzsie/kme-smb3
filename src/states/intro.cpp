#include "intro.hpp"

#include "basegame.hpp"
#include "../engine.hpp"

namespace kme {
Intro::Factory Intro::create() {
  return [](BaseState*, Engine* engine) -> BaseState* {
    return new Intro(engine);
  };
}

void Intro::enter() {}

void Intro::exit() {}

void Intro::pause() {
  paused = true;
}

void Intro::resume() {
  paused = false;
}

void Intro::update(float delta) {
  if (not paused) {
    engine->pushState(BaseGame::create());
    pause();
  }
}

void Intro::draw(float delta) {}

Intro::Intro(Engine* engine) : BaseState(nullptr, engine) {}
}
