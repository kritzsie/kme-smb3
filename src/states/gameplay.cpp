#include "gameplay.hpp"

//#include "basegame/entity.hpp"
#include "basegame/ecs/components.hpp"

#include "../assetmanager.hpp"
#include "../engine.hpp"
#include "../util.hpp"

#include <cmath>
#include <iostream>
namespace kme {
Gameplay::Factory Gameplay::create() {
  return [=](BaseState* parent, Engine* engine) -> BaseState* {
    return new Gameplay(parent, engine);
  };
}

Gameplay::Gameplay(BaseState* parent, Engine* engine)
: BaseState(parent, engine)
, level(getBaseGame(), this) {
  framebuffer = new sf::RenderTexture;
  framebuffer->create(480, 270);

  camera_pos = Vec2f(15.f, 8.4375f);
  camera_radius = Vec2f(15.f, 8.4375f);

  current_subworld = 0;

  inputs[Action::UP]       = 0;
  inputs[Action::DOWN]     = 0;
  inputs[Action::LEFT]     = 0;
  inputs[Action::RIGHT]    = 0;
  inputs[Action::JUMP]     = 0;
  inputs[Action::SPINJUMP] = 0;
  inputs[Action::RUN]      = 0;
  inputs[Action::SELECT]   = 0;
  inputs[Action::PAUSE]    = 0;

  keybinds[sf::Keyboard::Key::Up]     = Action::UP;
  keybinds[sf::Keyboard::Key::Down]   = Action::DOWN;
  keybinds[sf::Keyboard::Key::Left]   = Action::LEFT;
  keybinds[sf::Keyboard::Key::Right]  = Action::RIGHT;
  keybinds[sf::Keyboard::Key::Z]      = Action::RUN;
  keybinds[sf::Keyboard::Key::X]      = Action::JUMP;
  keybinds[sf::Keyboard::Key::C]      = Action::SPINJUMP;
  keybinds[sf::Keyboard::Key::Escape] = Action::PAUSE;

  axisbinds[std::tuple(0, sf::Joystick::Axis::X, Sign::MINUS)] = Action::LEFT;
  axisbinds[std::tuple(0, sf::Joystick::Axis::X, Sign::PLUS)]  = Action::RIGHT;
  axisbinds[std::tuple(0, sf::Joystick::Axis::Y, Sign::MINUS)] = Action::UP;
  axisbinds[std::tuple(0, sf::Joystick::Axis::Y, Sign::PLUS)]  = Action::DOWN;

  axisbinds[std::tuple(0, sf::Joystick::Axis::PovX, Sign::MINUS)] = Action::LEFT;
  axisbinds[std::tuple(0, sf::Joystick::Axis::PovX, Sign::PLUS)]  = Action::RIGHT;
  axisbinds[std::tuple(0, sf::Joystick::Axis::PovY, Sign::MINUS)] = Action::UP;
  axisbinds[std::tuple(0, sf::Joystick::Axis::PovY, Sign::PLUS)]  = Action::DOWN;

  buttonbinds[std::tuple(0, 0)] = Action::JUMP;
  buttonbinds[std::tuple(0, 1)] = Action::SPINJUMP;
  buttonbinds[std::tuple(0, 2)] = Action::RUN;
  buttonbinds[std::tuple(0, 3)] = Action::RUN;
  buttonbinds[std::tuple(0, 6)] = Action::SELECT;
  buttonbinds[std::tuple(0, 7)] = Action::PAUSE;

  playMusic("overworld.spc");
}

Gameplay::~Gameplay() {
  if (framebuffer != nullptr) {
    delete framebuffer;
  }
}

BaseGame* Gameplay::getBaseGame() {
  return dynamic_cast<BaseGame*>(parent);
}

bool Gameplay::handleInput(sf::Event::EventType type, const sf::Event& event) {
  switch (type) {
  case sf::Event::KeyPressed:
  case sf::Event::KeyReleased: {
    const auto& it = keybinds.find(event.key.code);
    if (it != keybinds.end()) {
      inputs[it->second] = sf::Keyboard::isKeyPressed(event.key.code);
    }
    break;
  }
  case sf::Event::JoystickMoved: {
    const auto& js = event.joystickMove;
    Sign sign = toSign(js.position);
    float pos = std::abs(js.position / 100.f);
    const auto& it_pos = axisbinds.find(std::tuple(js.joystickId, js.axis, Sign::PLUS));
    if (it_pos != axisbinds.end()) {
      inputs[it_pos->second] = sign == Sign::PLUS ? pos : 0;
    }
    const auto& it_neg = axisbinds.find(std::tuple(js.joystickId, js.axis, Sign::MINUS));
    if (it_neg != axisbinds.end()) {
      inputs[it_neg->second] = sign == Sign::MINUS ? pos : 0;
    }
  }
  case sf::Event::JoystickButtonPressed:
  case sf::Event::JoystickButtonReleased: {
    const auto& js = event.joystickButton;
    const auto& it = buttonbinds.find(std::tuple(js.joystickId, js.button));
    if (it != buttonbinds.end()) {
      inputs[it->second] = sf::Joystick::isButtonPressed(js.joystickId, js.button);
    }
    break;
  }
  default:
    break;
  }
  return true;
}

void Gameplay::enter() {
  Subworld& subworld = level.getSubworld(current_subworld);

  subworld.setSize(172, 27);

  // tiles
  Tilemap& tiles = subworld.getTiles();

  tiles[0][0] = TileID("wood_floor_0");
  for (int x = 1; x < 23; ++x) {
    tiles[x][0] = TileID("wood_floor_1");
  }
  tiles[23][0] = TileID("wood_floor_2");

  tiles[13][4] = TileID("brick_block");
  for (int x = 14; x < 18; ++x) {
    Tile prev = tiles[x - 1][4];
    if      (prev == TileID("brick_block")) tiles[x][4] = TileID("item_block");
    else if (prev == TileID("item_block"))  tiles[x][4] = TileID("brick_block");
  }

  // entities
  auto& entities = subworld.getEntities();

  auto player = subworld.player = subworld.camera_target = entities.create();
  entities.emplace<CInfo>(player, true, "PlayerMario");
  entities.emplace<CPosition>(player, Vec2f(2.f, 1.f));
  entities.emplace<CCollision>(player, 4.f / 16.f, 25.f / 16.f);
  entities.emplace<CFlags>(player);
  entities.emplace<CState>(player);
  entities.emplace<CTimer>(player);
  entities.emplace<CVelocity>(player);
  entities.emplace<CDirection>(player);
  entities.emplace<CRender>(player);
  entities.emplace<CAudio>(player);
}

void Gameplay::exit() {}

void Gameplay::pause() {}

void Gameplay::resume() {}

void Gameplay::update(float delta) {
  for (auto& input : inputs)
    input.second.update();

  Subworld& subworld = level.getSubworld(current_subworld);

  subworld.update(delta);

  ticktime += delta;
}

void Gameplay::draw(float delta) {
  Window* window = engine->getWindow();

  if (window != nullptr) {
    const Subworld& subworld = level.getSubworld(current_subworld);
    const EntityRegistry& entities = subworld.getEntities();
    const Vec2f& pos = entities.get<CPosition>(subworld.camera_target);
    Rect size = subworld.getSize();

    // position camera relative to world
    if (pos.x - 1.f > camera_pos.x) {
      camera_pos.x = pos.x - 1.f;
    }
    else if (pos.x + 1.f < camera_pos.x) {
      camera_pos.x = pos.x + 1.f;
    }

    if (pos.y - 1.5f > camera_pos.y) {
      camera_pos.y = pos.y - 1.5f;
    }
    else if (pos.y + 3.f < camera_pos.y) {
      camera_pos.y = pos.y + 3.f;
    }

    // restrict camera to world boundaries
    camera_pos.x = std::clamp(
      camera_pos.x,
      camera_radius.x + size.x,
      size.width - camera_radius.x + size.x
    );
    camera_pos.y = std::clamp(
      camera_pos.y,
      camera_radius.y + size.y,
      size.height - camera_radius.y + size.y
    );

    // snap camera to integer coordinates on screen
    camera_pos = toScreen(camera_pos);
    camera_pos.x = util::round(camera_pos.x);
    camera_pos.y = util::round(camera_pos.y);
    camera_pos = fromScreen(camera_pos);

    // set view to camera
    sf::View view = framebuffer->getView();
    view.setCenter(toScreen(camera_pos));
    framebuffer->setView(view);

    drawBackground(sf::Color(0x6898F8FF));
    drawBackground("overworldblockstop", Vec2f(128.f, 11.f), Vec2f(0.375f, 0.25f));
    drawBackground("cloudlayer", Vec2f(0.f, -224.f), Vec2f(0.75f, 0.125f));

    drawTiles();

    drawEntities();

    // trusty debug rectangle (draws a rectangle around the camera viewport)
    /*
    sf::RectangleShape rect;
    Vec2f pos_rect = (camera_pos - camera_radius) * 16.f + Vec2f(1.f, 269.f);
    rect.setPosition(Vec2f(pos_rect.x, -pos_rect.y));
    rect.setSize(Vec2f(478.f, 268.f));
    rect.setFillColor(sf::Color(0));
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(2.f);
    framebuffer->draw(rect);
    */

    framebuffer->display();

    window->getFramebuffer()->draw(sf::Sprite(framebuffer->getTexture()));
  }

  rendertime += delta;
}

void Gameplay::drawBackground(sf::Color color) {
  framebuffer->clear(color);
}

// NOTE: this function could use a few improvements
void Gameplay::drawBackground(std::string texture, Vec2f offset, Vec2f parallax,
                              bool vertical_tiling) {
  const sf::Texture& sf_texture = gfx.getTexture(texture);
  Vec2f size = static_cast<sf::Vector2f>(sf_texture.getSize());

  Vec2f tiling_ratio = Vec2f(480.f / size.x, 270.f / size.y);

  Vec2f start = camera_pos - camera_radius;
  Vec2f bg_start = start;
  bg_start.x = bg_start.x * (1.f - parallax.x);
  bg_start.y = bg_start.y * (1.f - parallax.y);
  Vec2f bg_end = bg_start + camera_radius * 2.f;
  bg_start.x = bg_start.x * 16.f / 480.f * tiling_ratio.x;
  bg_start.y = bg_start.y * 16.f / 270.f * tiling_ratio.y;
  bg_end.x = bg_end.x * 16.f / 480.f * tiling_ratio.x;
  bg_end.y = bg_end.y * 16.f / 270.f * tiling_ratio.y;

  if (vertical_tiling == true) {
    for (float y = std::floor(bg_start.y + offset.y / size.y); y < std::floor(bg_end.y + offset.y / size.y) + 1.f; y += 1.f)
    for (float x = std::floor(bg_start.x + offset.x / size.x); x < std::floor(bg_end.x + offset.x / size.x) + 1.f; x += 1.f) {
      sf::Sprite sprite(sf_texture);
      Vec2f pos = Vec2f((start.x * parallax.x * 16.f) + x * size.x,
                      -((start.y * parallax.y * 16.f) + y * size.y + size.y));
      pos.x -= offset.x;
      pos.y += offset.y;
      sprite.setPosition(pos);
      framebuffer->draw(sprite);
    }
  }
  else {
    for (float x = std::floor(bg_start.x + offset.x / size.x); x < std::floor(bg_end.x + offset.x / size.x) + 1.f; x += 1.f) {
      sf::Sprite sprite(sf_texture);
      Vec2f pos = Vec2f((start.x * parallax.x * 16.f) + x * size.x,
                      -((start.y * parallax.y * 16.f) + size.y));
      pos.x -= offset.x;
      pos.y += offset.y;
      sprite.setPosition(pos);
      framebuffer->draw(sprite);
    }
  }
}

// begin ugly
static Rect<float> rectFromBox(Vec2f pos, Vec2f radius) {
  return Rect<float>(pos - radius, 2.f * radius);
}

static Rect<float> regionFromRect(Rect<float> rect) {
  Rect<float> border(-0.5f, -0.5f, 1.f, 1.f);
  return rect + border;
}

static Rect<int> viewportFromRegion(Rect<float> region) {
  return static_cast<Rect<int>>(region.map(std::roundf));
}
// end ugly

void Gameplay::drawTiles() {
  Rect<int> region = viewportFromRegion(regionFromRect(rectFromBox(camera_pos, camera_radius)));

  for (int y = region.y; y < region.y + region.height; ++y)
  for (int x = region.x; x < region.x + region.width;  ++x) {
    Tile tile = level.getSubworld(current_subworld).getTiles()[x][y];
    if (tile != Tile::none) {
      TileDef tiledef = getBaseGame()->level_tile_data.getTileDef(tile);
      std::size_t frame = tiledef.getFrameOffset(rendertime);
      std::string texture = tiledef.getFrame(frame).texture;
      if (texture != "") {
        Vec2<int> pos = tile.getPos();
        sf::Sprite sprite(gfx.getTile(texture), tiledef.getFrame(frame).cliprect);
        sprite.setPosition(sf::Vector2f(pos.x * 16.f, -(pos.y * 16.f + 16.f)));
        framebuffer->draw(sprite);
      }
    }
  }
}

void Gameplay::drawEntities() {
  const Subworld& subworld = level.getSubworld(current_subworld);
  const EntityData& entity_data = getBaseGame()->entity_data;
  const EntityRegistry& entities = subworld.getEntities();

  auto view = entities.view<CInfo, CPosition, CRender, CDirection>();
  for (auto entity : view) {
    const auto& info = view.get<const CInfo>(entity);
    const Vec2f& pos = view.get<const CPosition>(entity);
    const auto& render = view.get<const CRender>(entity);
    const Sign& direction = view.get<const CDirection>(entity);

    const RenderFrame& frame = entity_data.getRenderStates(info.type)->getFrame(render.state);

    std::string spritename = frame.texture;
    if (spritename != "") {
      sf::Sprite sprite(gfx.getSprite(spritename), frame.cliprect);
      Vec2f scale = Vec2f(direction * render.scale.x, render.scale.y);
      Vec2f offset = Vec2f(
        scale.x * frame.offset.x,
        scale.y * (frame.offset.y + frame.cliprect.height)
      );
      Vec2f pos_render = toScreen(pos) - offset;
      pos_render.x = std::floor(pos_render.x + 0.5f);
      pos_render.y = std::floor(pos_render.y + 0.5f);
      sprite.setPosition(pos_render);
      sprite.setScale(scale);
      framebuffer->draw(sprite);
    }
  }
}

Vec2f Gameplay::fromScreen(Vec2f pos) {
  return Vec2f(pos.x, -pos.y) / 16.f;
}

Vec2f Gameplay::toScreen(Vec2f pos) {
  return Vec2f(pos.x, -pos.y) * 16.f;
}

Vec2f Gameplay::fromTile(Vec2i pos) {
  return static_cast<Vec2f>(pos);
}

Vec2i Gameplay::toTile(Vec2f pos) {
  return static_cast<Vec2i>(pos);
}

bool Gameplay::playMusic(std::string name) {
  return engine->music->open(name);
}

void Gameplay::stopMusic() {
  engine->music->stop();
}

std::size_t Gameplay::playSound(std::string name) {
  return engine->sound->play(name);
}

bool Gameplay::stopSound(std::size_t index) {
  return engine->sound->stop(index);
}

std::size_t Gameplay::playSoundLoop(std::string name) {
  return engine->sound->playLoop(name);
}

void Gameplay::stopSoundLoop(std::size_t index) {
  engine->sound->setLoop(index, false);
}
}
