#pragma once

#include "types.hpp"
#include "util/file.hpp"

#include <physfs.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <string>

namespace kme {
class AssetManager {
protected:
  AssetManager(const StringList& extensions);

private:
  AssetManager(const AssetManager&) = delete;
  AssetManager& operator =(const AssetManager&) = delete;

public:
  virtual ~AssetManager() = default;

protected:
  bool load(std::string folder, std::string name);
  virtual bool onLoad(util::FileInputStream& ifs, std::string folder, std::string name) = 0;

public:
  const StringList extensions;
};

class GFXAssets : public AssetManager {
public:
  static inline sf::Texture none;
  static inline sf::Texture missing;

  static GFXAssets& getInstance();

  const sf::Texture& getSprite(std::string name);
  const sf::Texture& getTexture(std::string name);
  const sf::Texture& getTile(std::string name);

  bool loadSprite(std::string name);
  bool loadTile(std::string name);
  bool loadTexture(std::string name);

private:
  GFXAssets();
  GFXAssets(const GFXAssets&) = delete;
  GFXAssets& operator =(const GFXAssets&) = delete;

  bool onLoad(util::FileInputStream& ifs, std::string folder, std::string name) final;

  StringTable<StringTable<sf::Texture>> assets;
};

class SFXAssets : public AssetManager {
public:
  static inline sf::SoundBuffer none;
  static inline sf::SoundBuffer missing;

  static SFXAssets& getInstance();

  const sf::SoundBuffer& getSound(std::string name);

  bool loadSound(std::string name);

private:
  SFXAssets();
  SFXAssets(const SFXAssets&) = delete;
  SFXAssets& operator =(const SFXAssets&) = delete;

  bool onLoad(util::FileInputStream& ifs, std::string folder, std::string name) final;

private:
  StringTable<sf::SoundBuffer> sounds;
};

extern GFXAssets& gfx;
extern SFXAssets& sfx;
}
