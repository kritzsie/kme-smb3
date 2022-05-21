#include "assetmanager.hpp"

#include "types.hpp"
#include "util/file.hpp"

#include <physfs.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <string>
#include <utility>

#include "assets/missing_texture_png.h"

namespace kme {
static const StringList gfx_extensions {
  "", "hdr", "png", "psd", "tga", "bmp", "gif", "pic", "jpg", "jpeg"
};

static const StringList sfx_extensions {
  "", "wav", "flac", "ogg"
};

GFXAssets& gfx = GFXAssets::getInstance();
SFXAssets& sfx = SFXAssets::getInstance();

// begin AssetManager
AssetManager::AssetManager(const StringList& extensions) : extensions(extensions) {}

bool AssetManager::load(std::string folder, std::string name) {
  for (std::string ext : extensions) {
    std::string path = "/" + folder + "/" + (ext.empty() ? name : name + "." + ext);
    if (PHYSFS_exists(path.c_str())) {
      util::FileInputStream ifs;

      ifs.open(path);

      return onLoad(ifs, folder, name);
    }
  }

  return false;
}
// end AssetManager

// begin GFXAssets
GFXAssets::GFXAssets() : AssetManager(gfx_extensions) {
  GFXAssets::missing.loadFromMemory(missing_texture_png, sizeof(missing_texture_png));

  assets["sprites" ][""] = GFXAssets::none;
  assets["tiles"   ][""] = GFXAssets::none;
  assets["textures"][""] = GFXAssets::none;
}

GFXAssets& GFXAssets::getInstance() {
  static GFXAssets instance;

  return instance;
}

bool GFXAssets::onLoad(util::FileInputStream& ifs, std::string folder, std::string name) {
  sf::Texture texture;

  texture.loadFromStream(ifs);
  texture.setRepeated(true);
  assets[folder][name] = std::move(texture);

  return true;
}

bool GFXAssets::loadSprite(std::string name) {
  return load("sprites", name);
}

bool GFXAssets::loadTexture(std::string name) {
  return load("textures", name);
}

bool GFXAssets::loadTile(std::string name) {
  return load("tiles", name);
}

const sf::Texture& GFXAssets::getTexture(std::string name) {
  try {
    return assets["textures"].at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTexture(name)) {
      return assets["textures"][name];
    }
  }

  return GFXAssets::missing;
}

const sf::Texture& GFXAssets::getTile(std::string name) {
  try {
    return assets["tiles"].at(name);
  }
  catch (const std::out_of_range&) {
    if (loadTile(name)) {
      return assets["tiles"][name];
    }
  }

  return getTexture(name);
}

const sf::Texture& GFXAssets::getSprite(std::string name) {
  try {
    return assets["sprites"].at(name);
  }
  catch (const std::out_of_range&) {
    if (loadSprite(name)) {
      return assets["sprites"][name];
    }
  }

  return getTile(name);
}
// end GFXAssets

// begin SFXAssets
bool SFXAssets::onLoad(util::FileInputStream& ifs, std::string, std::string name) {
  sf::SoundBuffer sound;

  sound.loadFromStream(ifs);
  sounds[name] = std::move(sound);

  return true;
}

bool SFXAssets::loadSound(std::string name) {
  return load("sounds", name);
}

const sf::SoundBuffer& SFXAssets::getSound(std::string name) {
  try {
    return sounds.at(name);
  }
  catch (const std::out_of_range&) {
    if (loadSound(name)) {
      return sounds[name];
    }
  }

  return SFXAssets::missing;
}

SFXAssets& SFXAssets::getInstance() {
  static SFXAssets instance;

  return instance;
}

SFXAssets::SFXAssets() : AssetManager(sfx_extensions) {
  sounds[""] = SFXAssets::none;
}
// end SFXAssets
}
