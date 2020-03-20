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
: BaseState(parent, engine), base_game(dynamic_cast<BaseGame*>(parent)) {
  framebuffer = new sf::RenderTexture;
  framebuffer->create(480, 270);

  subworld_current = 0;
}

Gameplay::~Gameplay() {}

void Gameplay::enter() {
  Tilemap& tiles = level.getSubworld(subworld_current).getTiles();

  tiles[0][0] = TileID("wood_floor_0");
  for (int x = 1; x < 23; ++x) {
    tiles[x][0] = TileID("wood_floor_1");
  }
  tiles[23][0] = TileID("wood_floor_2");

  tiles[10][5] = TileID("brick_block");
  for (int x = 11; x < 15; ++x) {
    Tile prev = tiles[x - 1][5];
    if      (prev == TileID("brick_block")) tiles[x][5] = TileID("item_block");
    else if (prev == TileID("item_block"))  tiles[x][5] = TileID("brick_block");
  }
}

void Gameplay::exit() {}

void Gameplay::pause() {}

void Gameplay::resume() {}

void Gameplay::update() {
  ticktime += engine->getTickTime().delta;
}

#warning Implement proper coordinate systems!
static Rect<float> regionFromCamera(Vec2f pos, Vec2f radius) {
  return Rect<float>(pos.x - radius.x, pos.y - radius.y,
                     2.f *   radius.x, 2.f *   radius.y)
       + Rect<float>(-0.5f, -0.5f,
                     1.f,   1.f);
}

static Rect<Int32> viewportFromRegion(Rect<float> rect) {
  return Rect<Int32>(std::round(rect.x),     std::round(rect.y),
                     std::round(rect.width), std::round(rect.height));
}

void Gameplay::draw() {
  Window* window = engine->getWindow();
  if (window != nullptr) {
    framebuffer->clear(sf::Color(0x6898F8FF));

    Vec2f camera_pos = Vec2f(15.f, 8.4375f);
    Vec2f camera_radius = Vec2f(15.f, 8.4375f);

    sf::View view = framebuffer->getView();
    view.setCenter(camera_pos.x * 16.f, -camera_pos.y * 16.f);
    framebuffer->setView(view);

    Rect<float> camera_region = regionFromCamera(camera_pos, camera_radius);
    drawTiles(viewportFromRegion(camera_region));

    framebuffer->display();

    window->getFramebuffer()->draw(sf::Sprite(framebuffer->getTexture()));
  }

  rendertime += engine->getRenderTime().delta;
}

void Gameplay::drawTiles(Rect<Int32> region) {
  for (Int32 y = region.y; y < region.y + region.height; ++y)
  for (Int32 x = region.x; x < region.x + region.width;  ++x) {
    Tile tile = level.getSubworld(subworld_current).getTiles()[x][y];
    if (tile != Tile::none) {
      TileDef tiledef = base_game->level_tiles.getTileDef(tile);
      std::size_t frame = tiledef.getFrameOffset(rendertime);
      std::string texture = tiledef.getFrame(frame).texture;
      if (texture != "") {
        Vec2<Int32> pos = tile.getPos();
        sf::Sprite sprite(gfx.getTile(texture), tiledef.getFrame(frame).cliprect);
        sprite.setPosition(sf::Vector2f(pos.x * 16.f, -(pos.y * 16.f + 16.f)));
        framebuffer->draw(sprite);
      }
    }
  }
}
}
