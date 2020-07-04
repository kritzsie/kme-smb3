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
: BaseState(parent, engine), level(getBaseGame()->entity_data, getBaseGame()->level_tile_data) {
  framebuffer = new sf::RenderTexture;
  framebuffer->create(480, 270);

  camera_pos = Vec2f(15.f, 8.4375f);
  camera_radius = Vec2f(15.f, 8.4375f);

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

  subworld.update(delta);

  ticktime += delta;
}

void Gameplay::draw(float delta) {
  Window* window = engine->getWindow();

  if (window != nullptr) {
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
    Vec2f pos = (camera_pos - camera_radius) * 16.f + Vec2f(1.f, 269.f);
    rect.setPosition(Vec2f(pos.x, -pos.y));
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
  const EntityComponentManager& entities = subworld.getEntities();

  for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
    const RenderFrame& frame = getBaseGame()->entity_data \
      .getRenderStates(entities.get<Type>(*iter))->getFrame();

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
  return static_cast<Vec2f>(pos);
}

Vec2i Gameplay::toTile(Vec2f pos) {
  return static_cast<Vec2i>(pos);
}
}
