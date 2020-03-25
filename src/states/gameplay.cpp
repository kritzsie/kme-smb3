#include "gameplay.hpp"

#include "../assetmanager.hpp"
#include "../engine.hpp"
#include "../util/math.hpp"

#include <ctgmath>

namespace kme {
Gameplay::Factory Gameplay::create() {
  return [=](BaseState* parent, Engine* engine) -> BaseState* {
    return new Gameplay(parent, engine);
  };
}

Gameplay::Gameplay(BaseState* parent, Engine* engine)
: BaseState(parent, engine) {
  framebuffer = new sf::RenderTexture;
  framebuffer->create(480, 270);

  camera_pos = Vec2f(15.f, 8.4375f);

  current_subworld = 0;
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
  case sf::Event::KeyReleased:
    switch (event.key.code) {
    case sf::Keyboard::Key::Up:
      pressedUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
      break;
    case sf::Keyboard::Key::Down:
      pressedDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
      break;
    case sf::Keyboard::Key::Left:
      pressedLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
      break;
    case sf::Keyboard::Key::Right:
      pressedRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
  return true;
}

void Gameplay::enter() {
  Subworld& subworld = level.getSubworld(current_subworld);

  // tiles
  Tilemap& tiles = subworld.getTiles();

  tiles[0][0] = TileID("wood_floor_0");
  for (int x = 1; x < 23; ++x) {
    tiles[x][0] = TileID("wood_floor_1");
  }
  tiles[23][0] = TileID("wood_floor_2");

  tiles[13][5] = TileID("brick_block");
  for (int x = 14; x < 18; ++x) {
    Tile prev = tiles[x - 1][5];
    if      (prev == TileID("brick_block")) tiles[x][5] = TileID("item_block");
    else if (prev == TileID("item_block"))  tiles[x][5] = TileID("brick_block");
  }

  // entities
  Entity mario = subworld.spawnEntity("player_mario", Vec2f(2.f, 1.f));
}

void Gameplay::exit() {}

void Gameplay::pause() {}

void Gameplay::resume() {}

void Gameplay::update(float delta) {
  Subworld& subworld = level.getSubworld(current_subworld);

  if (pressedUp or pressedDown or pressedLeft or pressedRight) {
    camera_pos += Vec2f(pressedRight - pressedLeft, pressedUp - pressedDown) * 16.f * delta;
  }

  subworld.update(delta);

  ticktime += delta;
}

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

static Vec2f getCameraRadius() {
  return Vec2f(15.f, 8.4375f);
}

void Gameplay::draw(float delta) {
  Window* window = engine->getWindow();

  if (window != nullptr) {
    sf::View view = framebuffer->getView();
    view.setCenter(toScreen(camera_pos));
    framebuffer->setView(view);

    drawBackground(sf::Color(0x6898F8FF));
    drawBackground("overworldblockstop", Vec2f(0.f, -11.f), Vec2f(0.5f, 0.25f));
    drawBackground("cloudlayer", Vec2f(0.f, 160.f), Vec2f(0.75f, 0.5f));

    Rect<float> camera_region = regionFromRect(rectFromBox(camera_pos, getCameraRadius()));
    drawTiles(viewportFromRegion(camera_region));

    drawEntities();

    framebuffer->display();

    window->getFramebuffer()->draw(sf::Sprite(framebuffer->getTexture()));
  }

  rendertime += delta;
}

void Gameplay::drawBackground(sf::Color color) {
  framebuffer->clear(color);
}

void Gameplay::drawBackground(std::string texture, Vec2f origin, Vec2f parallax) {
  sf::Sprite sprite(gfx.getTexture(texture));
  Vec2f pos = Vec2f(origin.x, -(origin.y + sprite.getTexture()->getSize().y));
  Vec2f offset = toScreen(camera_pos - getCameraRadius());
  offset.x *= parallax.x;
  offset.y *= parallax.y;
  pos += offset;
  sprite.setPosition(pos);
  framebuffer->draw(sprite);
}

void Gameplay::drawTiles(Rect<int> region) {
  for (int y = region.y; y < region.y + region.height; ++y)
  for (int x = region.x; x < region.x + region.width;  ++x) {
    Tile tile = level.getSubworld(current_subworld).getTiles()[x][y];
    if (tile != Tile::none) {
      TileDef tiledef = getBaseGame()->level_tiles.getTileDef(tile);
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
  const EntityComponentManager& entities = subworld.getEntities();

  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    const RenderFrame& frame = getBaseGame()->entity_data.getRenderStates(entities.get<Type>(*iter))->getFrame();
    std::string texture = frame.texture;
    if (texture != "") {
      sf::Sprite sprite(gfx.getSprite(texture), frame.cliprect);
      Vec2f offset = frame.offset + Vec2f(0.f, frame.cliprect.height);
      sprite.setPosition(toScreen(entities.get<Position>(*iter)) - offset);
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
  return Vec2f(pos.x, pos.y);
}

Vec2i Gameplay::toTile(Vec2f pos) {
  return Vec2i(pos.x, pos.y);
}
}
