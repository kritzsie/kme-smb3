#include "gameplay.hpp"

#include "../assetmanager.hpp"
#include "../engine.hpp"
#include "../graphics.hpp"
#include "../math.hpp"
#include "../renderer.hpp"
#include "../util.hpp"
#include "basegame/ecs/components.hpp"
#include "basegame/levelloader.hpp"
#include "basegame.hpp"

#include <iomanip>
#include <map>
#include <vector>

#include <cmath>

namespace kme {
using namespace vec2_aliases;

Gameplay::Factory Gameplay::create(std::size_t worldnum, std::size_t levelnum) {
  return [=](BaseState* parent, Engine* engine) -> BaseState* {
    return new Gameplay(parent, engine, worldnum, levelnum);
  };
}

Gameplay::Gameplay(BaseState* parent, Engine* engine, std::size_t worldnum, std::size_t levelnum)
: BaseState(parent, engine), worldnum(worldnum), levelnum(levelnum), level(getBaseGame(), this) {
  framebuffer = new sf::RenderTexture;
  scene = new sf::RenderTexture;
  hud = new sf::RenderTexture;

  framebuffer->create(480, 270);
  scene->create(480, 270);
  hud->create(480, 270);

  inputs.actions[Action::UP]       = 0;
  inputs.actions[Action::DOWN]     = 0;
  inputs.actions[Action::LEFT]     = 0;
  inputs.actions[Action::RIGHT]    = 0;
  inputs.actions[Action::JUMP]     = 0;
  inputs.actions[Action::SPINJUMP] = 0;
  inputs.actions[Action::RUN]      = 0;
  inputs.actions[Action::SELECT]   = 0;
  inputs.actions[Action::PAUSE]    = 0;

  inputs.keys[sf::Keyboard::Key::Left]   = Action::LEFT;
  inputs.keys[sf::Keyboard::Key::Right]  = Action::RIGHT;
  inputs.keys[sf::Keyboard::Key::Up]     = Action::UP;
  inputs.keys[sf::Keyboard::Key::Down]   = Action::DOWN;
  inputs.keys[sf::Keyboard::Key::Z]      = Action::RUN;
  inputs.keys[sf::Keyboard::Key::X]      = Action::JUMP;
  inputs.keys[sf::Keyboard::Key::C]      = Action::SPINJUMP;
  inputs.keys[sf::Keyboard::Key::Escape] = Action::PAUSE;

  inputs.axes[std::tuple(0, sf::Joystick::Axis::X, Sign::MINUS)] = Action::LEFT;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::X, Sign::PLUS)]  = Action::RIGHT;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::Y, Sign::MINUS)] = Action::UP;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::Y, Sign::PLUS)]  = Action::DOWN;

  inputs.axes[std::tuple(0, sf::Joystick::Axis::PovX, Sign::MINUS)] = Action::LEFT;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::PovX, Sign::PLUS)]  = Action::RIGHT;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::PovY, Sign::MINUS)] = Action::UP;
  inputs.axes[std::tuple(0, sf::Joystick::Axis::PovY, Sign::PLUS)]  = Action::DOWN;

  inputs.buttons[std::tuple(0, 0)] = Action::JUMP;
  inputs.buttons[std::tuple(0, 1)] = Action::SPINJUMP;
  inputs.buttons[std::tuple(0, 2)] = Action::RUN;
  inputs.buttons[std::tuple(0, 3)] = Action::RUN;
  inputs.buttons[std::tuple(0, 6)] = Action::SELECT;
  inputs.buttons[std::tuple(0, 7)] = Action::PAUSE;
}

Gameplay::~Gameplay() {
  delete framebuffer;
  delete scene;
  delete hud;
}

BaseGame* Gameplay::getBaseGame() {
  return dynamic_cast<BaseGame*>(parent);
}

bool Gameplay::handleInput(sf::Event::EventType type, const sf::Event& event) {
  if (not paused) {
    inputs.update(type, event);
  }

  return not paused;
}

void Gameplay::enter() {
  LevelLoader loader(worldnum, levelnum);
  loader.load(level);

  Subworld& subworld = level.getSubworld(current_subworld);

  // entities
  auto& entities = subworld.getEntities();

  auto player = entities.create();
  auto player_powerup = Powerup::NONE;
  auto player_state = EState::IDLE;
  const auto& player_hitbox = getBaseGame() \
    ->entity_data.getHitboxes("Player").at(player_powerup).at(player_state);
  entities.emplace<CInfo>(player, "Player");
  entities.emplace<CPosition>(player, Vec2f(2.f, 1.f));
  entities.emplace<CPowerup>(player, player_powerup);
  entities.emplace<CState>(player, player_state);
  entities.emplace<CCollision>(player, player_hitbox);
  entities.emplace<CVelocity>(player);
  entities.emplace<CDirection>(player);
  entities.emplace<CFlags>(player);
  entities.emplace<CCounters>(player);
  entities.emplace<CTimers>(player);
  entities.emplace<CRender>(player);
  entities.emplace<CAudio>(player);
  subworld.player = player;

  auto camera = entities.create();
  entities.emplace<CInfo>(camera, "Camera", player);
  entities.emplace<CPosition>(camera, Vec2f(15.f, 0.f));
  entities.emplace<CCollision>(camera, Hitbox(15.f, 16.875f));
  subworld.camera = camera;

  auto goalcard = entities.create();
  entities.emplace<CInfo>(goalcard, "GoalCard");
  entities.emplace<CPosition>(goalcard, Vec2f(164.5f, 5.f));
  entities.emplace<CCollision>(goalcard, Hitbox(0.5f, 1.f));
  entities.emplace<CFlags>(goalcard);
  entities.emplace<CState>(goalcard);
  entities.emplace<CRender>(goalcard);

  /*
  auto mushroom = entities.create();
  entities.emplace<CInfo>(mushroom, "Mushroom");
  entities.emplace<CFlags>(mushroom, EFlags::POWERUP | EFlags::NOFRICTION);
  entities.emplace<CPosition>(mushroom, Vec2f(15.5f, 7.f));
  entities.emplace<CVelocity>(mushroom, Vec2f(4.f, 0.f));
  entities.emplace<CCollision>(mushroom, Hitbox(0.5f, 1.f));
  entities.emplace<CPowerup>(mushroom, Powerup::MUSHROOM);
  entities.emplace<CDirection>(mushroom);
  entities.emplace<CState>(mushroom);
  entities.emplace<CRender>(mushroom);

  auto goomba = entities.create();
  entities.emplace<CInfo>(goomba, "Goomba");
  entities.emplace<CFlags>(goomba, EFlags::ENEMY | EFlags::NOFRICTION);
  entities.emplace<CState>(goomba, EState::WALK);
  entities.emplace<CPosition>(goomba, Vec2f(15.5f, 1.f));
  entities.emplace<CVelocity>(goomba, Vec2f(-2.f, 0.f));
  entities.emplace<CCollision>(goomba, Hitbox(0.5f, 0.75f));
  entities.emplace<CDirection>(goomba);
  entities.emplace<CTimers>(goomba);
  entities.emplace<CRender>(goomba);

  auto pswitch = entities.create();
  entities.emplace<CInfo>(pswitch, "PSwitch");
  entities.emplace<CPosition>(pswitch, Vec2f(16.5f, 4.f));
  entities.emplace<CCollision>(pswitch, Hitbox(0.5f, 1.f));
  entities.emplace<CFlags>(pswitch, EFlags::SOLID);
  entities.emplace<CVelocity>(pswitch);
  entities.emplace<CState>(pswitch);
  entities.emplace<CRender>(pswitch);
  */

  const auto& theme = getBaseGame()->themes.at(subworld.getTheme());
  playMusic(theme.music);
}

void Gameplay::exit() {}

void Gameplay::pause() {
  paused = true;
}

void Gameplay::resume() {
  paused = false;
}

void Gameplay::update(float delta) {
  if (not paused) {
    for (auto& action : inputs.actions) {
      action.second.update();
    }

    suspended_previous = suspended;

    Subworld& subworld = level.getSubworld(current_subworld);
    if (not suspended) {
      subworld.update(delta);

      if (level.timer > 0.f) {
        float timer_old = level.timer;
        level.timer = std::max(level.timer - delta, 0.f);
        if (level.timer < 100.f and timer_old >= 100.f) {
          engine->sound->play("hurry");
          engine->music->setTempo(engine->music->getTempo() * 4.f / 3.f);
          engine->music->play();
        }
      }
    }

    if (suspended != suspended_previous and not suspended_previous) {
      engine->sound->stop();
    }

    ticktime += delta;
  }
}

void Gameplay::suspend() {
  suspended = true;
}

void Gameplay::unsuspend() {
  suspended = false;
}

bool Gameplay::isSuspended() const {
  return suspended;
}

void Gameplay::draw(float delta) {
  Window* window = engine->getWindow();

  if (window != nullptr) {
    const Subworld& subworld = level.getSubworld(current_subworld);
    const EntityRegistry& entities = subworld.getEntities();
    const auto& pos = entities.get<CPosition>(subworld.camera).value;
    const auto& coll = entities.get<CCollision>(subworld.camera);
    Rect<float> aabb = coll.hitbox.toAABB(pos);

    // snap camera to integer coordinates on screen
    aabb.pos = fromScreen(fp::map(util::round, toScreen(aabb.pos)));

    // set view to camera
    sf::View view = scene->getView();
    view.setCenter(toScreen(geo::midpoint(aabb)));
    scene->setView(view);

    const auto& theme = getBaseGame()->themes.at(subworld.getTheme());
    drawBackground(theme.background);
    for (const auto& it : theme.layers) {
      drawBackground(it.second);
    }

    drawTiles();
    drawEntities();
    drawHUD();

    scene->display();
    hud->display();

    framebuffer->draw(sf::Sprite(scene->getTexture()));
    framebuffer->draw(sf::Sprite(hud->getTexture()));
    framebuffer->display();

    window->getFramebuffer()->draw(sf::Sprite(framebuffer->getTexture()));
  }

  rendertime += delta;
}

void Gameplay::drawBackground(Color color) {
  scene->clear(color);
}

void Gameplay::drawBackground(Layer layer) {
  drawBackground(layer.background, layer.offset, layer.parallax, layer.repeat_y);
}

// NOTE: this function could use a few improvements
void Gameplay::drawBackground(std::string name, Vec2f offset, Vec2f parallax,
                              bool tile_vertically) {
  const RenderFrames& background = getBaseGame()->backgrounds.at(name);
  const RenderFrame& frame = background.getFrame(background.getFrameOffset(rendertime));
  const sf::Texture& texture = gfx.getTexture(frame.texture);
  Vec2f size = static_cast<sf::Vector2f>(texture.getSize());
  Vec2f tiling_ratio = Vec2f(480.f / size.x, 270.f / size.y);

  const Subworld& subworld = level.getSubworld(current_subworld);
  const EntityRegistry& entities = subworld.getEntities();
  const auto& pos = entities.get<CPosition>(subworld.camera).value;
  const auto& coll = entities.get<CCollision>(subworld.camera);
  const auto aabb = coll.hitbox.toAABB(pos);

  Vec2f origin = aabb.pos;
  Vec2f bg_begin = origin;
  bg_begin.x = bg_begin.x * (1.f - parallax.x);
  bg_begin.y = bg_begin.y * (1.f - parallax.y);
  Vec2f bg_end = bg_begin + aabb.size;
  bg_begin.x = bg_begin.x * 16.f / 480.f * tiling_ratio.x;
  bg_begin.y = bg_begin.y * 16.f / 270.f * tiling_ratio.y;
  bg_end.x = bg_end.x * 16.f / 480.f * tiling_ratio.x;
  bg_end.y = bg_end.y * 16.f / 270.f * tiling_ratio.y;

  if (tile_vertically == true) {
    Vec2f begin(
      std::floor(bg_begin.x + offset.x / size.x),
      std::floor(bg_begin.y + offset.y / size.y)
    );
    Vec2f end(
      std::floor(bg_end.x + offset.x / size.x) + 1.f,
      std::floor(bg_end.y + offset.y / size.y) + 1.f
    );
    for (float y = begin.y; y < end.y; y += 1.f)
    for (float x = begin.x; x < end.x; x += 1.f) {
      sf::Sprite sprite(texture);
      Vec2f pos = Vec2f((origin.x * parallax.x * 16.f) + x * size.x,
                      -((origin.y * parallax.y * 16.f) + y * size.y + size.y));
      pos -= offset;
      sprite.setPosition(pos);
      scene->draw(sprite);
    }
  }
  else {
    float begin = std::floor(bg_begin.x + offset.x / size.x);
    float end = std::floor(bg_end.x + offset.x / size.x) + 1.f;
    for (float x = begin; x < end; x += 1.f) {
      sf::Sprite sprite(texture);
      Vec2f pos = Vec2f((origin.x * parallax.x * 16.f) + x * size.x,
                      -((origin.y * parallax.y * 16.f) + size.y));
      pos -= offset;
      sprite.setPosition(pos);
      scene->draw(sprite);
    }
  }
}

// begin ugly
static Rect<float> regionFromRect(Rect<float> rect) {
  static constexpr Rect<float> border(-0.5f, -0.5f, 1.f, 1.f);
  return Rect<float>(rect.pos + border.pos, rect.size + border.size);
}

static Rect<int> viewportFromRegion(Rect<float> region) {
  return static_cast<Rect<int>>(fp::map(std::roundf, region));
}
// end ugly

void Gameplay::drawTiles() {
  const Subworld& subworld = level.getSubworld(current_subworld);
  const EntityRegistry& entities = subworld.getEntities();
  const auto& pos = entities.get<CPosition>(subworld.camera).value;
  const auto& coll = entities.get<CCollision>(subworld.camera);

  Rect<int> region = viewportFromRegion(regionFromRect(coll.hitbox.toAABB(pos)));
  const auto& tilemap = level.getSubworld(current_subworld).getTilemap();
  const auto& layers = tilemap.getLayers();

  for (auto iter = layers.rbegin(); iter != layers.rend(); ++iter)
  for (int y = region.y; y < region.y + region.height; ++y)
  for (int x = region.x; x < region.x + region.width;  ++x) {
    Tile tile(iter->first, x, y);
    TileID tileid = tilemap.getTile(tile);
    if (tileid != Tilemap::notile) {
      TileDef tiledef = getBaseGame()->level_tile_data.getTileDef(tileid);
      std::size_t frame = tiledef.getFrameOffset(rendertime);
      std::string texture = tiledef.getFrame(frame).texture;
      if (texture != "") {
        sf::Sprite sprite(gfx.getTile(texture), tiledef.getFrame(frame).cliprect);
        sprite.setPosition(toScreen(Vec2f(tile.pos.x, tile.pos.y + 1)));
        scene->draw(sprite);
      }
    }
  }
}

void Gameplay::drawEntities() {
  const Subworld& subworld = level.getSubworld(current_subworld);
  const EntityData& entity_data = getBaseGame()->entity_data;
  const EntityRegistry& entities = subworld.getEntities();

  auto view = entities.view<CInfo, CPosition, CRender>();
  for (auto entity : view) {
    auto timer_view = entities.view<CTimers>();
    if (timer_view.contains(entity)) {
      const auto& timers = timer_view.get<const CTimers>(entity);
      if (timers.i_frames != 0.f
      and std::fmod(rendertime, 0.125f) > 0.0625f) {
        continue; // make player blink on i-frames
      }
    }

    const auto& info = view.get<const CInfo>(entity);
    const auto& pos = view.get<const CPosition>(entity).value;
    const auto& render = view.get<const CRender>(entity);

    const auto& frame = entity_data.getRenderStates(info.type).getFrame(render.state);

    Sign direction = Sign::PLUS;
    auto direction_view = entities.view<CDirection>();
    if (direction_view.contains(entity)) {
      direction = direction_view.get<const CDirection>(entity).value;
    }

    std::string texture = frame.texture;
    if (texture != "") {
      sf::Sprite sprite(gfx.getSprite(texture), frame.cliprect);
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
      scene->draw(sprite);
    }
  }
}

void Gameplay::drawHUD() {
  hud->clear(Color(0));

  const auto& subworld = level.getSubworld(current_subworld);
  const auto& entities = subworld.getEntities();

  const auto& counters = entities.get<CCounters>(subworld.player);

  std::stringstream world;
  world << util::highASCII("abcd") << worldnum << '-' << levelnum;

  std::stringstream p_meter;
  for (std::size_t i = 0; i < 6; ++i) {
    p_meter << (counters.p_meter > i ? util::highASCII('>') : '>');
  }
  if (counters.p_meter > 6.f
  and std::fmod(rendertime, 0.25f) > 0.125f) {
    p_meter << util::highASCII("()");
  }
  else {
    p_meter << "()";
  }

  std::stringstream coins;
  coins << "$" << std::setw(2) << getBaseGame()->getCoins();

  std::stringstream mario;
  mario << util::highASCII("ABx") << std::setw(2) << getBaseGame()->getLives();

  std::stringstream score;
  score << std::internal << std::setw(7) << std::setfill('0') << getBaseGame()->getScore();

  std::stringstream timer;
  timer << "@" << std::fixed << std::internal
           << std::setprecision(0) << std::setw(3) << std::setfill('0')
           << int(std::ceil(level.timer));

  TextStyle align_left("smb3_sbfont");
  drawText(hud, world.str(), Vec2f(16, 16), align_left);
  drawText(hud, mario.str(), Vec2f(16, 24), align_left);

  TextStyle align_right("smb3_sbfont", TextStyle::Flags::ALIGN_RIGHT);
  drawText(hud, timer.str(), Vec2f(48, 16), align_right);
  drawText(hud, coins.str(), Vec2f(16, 16), align_right);
  drawText(hud, score.str(), Vec2f(16, 24), align_right);

  TextStyle align_bottom("smb3_sbfont", TextStyle::Flags::ALIGN_BOTTOM);
  drawText(hud, p_meter.str(), Vec2f(64, 6), align_bottom);
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
