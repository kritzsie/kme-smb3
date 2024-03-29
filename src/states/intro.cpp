#include "intro.hpp"

#include "../engine.hpp"
#include "mainmenu.hpp"

namespace kme {
Intro::Factory Intro::create() {
  return [](BaseState*, Engine* engine) -> BaseState* {
    return new Intro(engine);
  };
}

Intro::Intro(Engine* engine) : BaseState(nullptr, engine) {}

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
    engine->popState();
    engine->pushState(MainMenu::create());
    pause();
  }
}

void Intro::draw(float delta) {}
}
