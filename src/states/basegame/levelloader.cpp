#include "levelloader.hpp"

#include "../../types.hpp"
#include "../../util.hpp"
#include "tiledefs.hpp"
#include "tilemap.hpp"

#include <json/reader.h>
#include <json/value.h>

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace kme {
LevelLoader::LevelLoader(std::size_t world, std::size_t level) {
  Json::Reader reader;

  std::stringstream path;
  path << "/maps/" << world << "-" << level;
  StringList files = util::getFiles(path.str());

  for (const auto& filename : files) {
    std::size_t subworld_id = std::stoi(filename);
    SubworldData& subworld_data = subworlds[subworld_id];

    std::unordered_map<std::size_t, TileType> tileset_types;

    Json::Value root;
    if (reader.parse(util::readFile(util::join({path.str(), filename}, "/")).data(), root)) {
      subworld_data.bounds.width = root["width"].asInt();
      subworld_data.bounds.height = root["height"].asInt();
    }
    else {
      throw std::runtime_error(filename + " " + reader.getFormattedErrorMessages());
    }

    for (const auto& tileset : root["tilesets"]) {
      std::size_t firstgid = tileset["firstgid"].asInt();
      std::string source = tileset["source"].asString();
      std::string tileset_path = util::join({path.str(), source}, "/");

      Json::Value tileset_root;
      if (reader.parse(util::readFile(tileset_path).data(), tileset_root)) {
        for (const auto& tiles : tileset_root["tiles"]) {
          std::size_t id = tiles["id"].asInt() + firstgid;
          tileset_types[id] = tiles["type"].asString();
        }
      }
    }

    std::size_t tilelayer_count = 0;
    for (const auto& layer : root["layers"]) {
      if (layer["type"] == "tilelayer") {
        tilelayer_count += 1;
      }
    }

    for (const auto& properties : root["properties"]) {
      if (properties["name"] == "theme") {
        subworld_data.theme = properties["value"].asString();
      }
    }

    for (const auto& layer : root["layers"]) {
      if (layer["type"] == "tilelayer") {
        tilelayer_count -= 1;
        auto layer_data = util::base64_decode(layer["data"].asString());
        for (std::size_t i = 0; i < layer_data.size(); i += 4) {
          std::size_t id = 0;
          // convert to little-endian
          for (std::size_t j = 0; j < 4; ++j) {
            id += static_cast<UInt8>(layer_data[i + j]) << (j * 8);
          }
          int x = (i / 4) % subworld_data.bounds.width;
          int y = subworld_data.bounds.height - (i / 4) / subworld_data.bounds.width - 1;
          auto it = tileset_types.find(id);
          if (it != tileset_types.end()) {
            subworld_data.tilemap.setTile(tilelayer_count, x, y, it->second);
          }
        }
      }
      else if (layer["type"] == "objectgroup") {
        for (auto object : layer["objects"]) {
          auto aabb = Rect<float>(
            object["x"].asFloat() / 16.f, subworld_data.bounds.height - object["y"].asFloat() / 16.f,
            object["width"].asFloat() / 16.f, object["height"].asFloat() / 16.f
          );
          auto pos = Vec2f(aabb.x + aabb.width / 2, aabb.y);
          auto it = tileset_types.find(object["gid"].asInt());
          if (it != tileset_types.end()) {
            subworld_data.entities.types.push_back(it->second);
            subworld_data.entities.pos.push_back(pos);
          }
        }
      }
      else if (layer["type"] == "imagelayer") {
        if (layer["name"] == "Water Layer") {
          subworld_data.water_height = std::round(subworld_data.bounds.height - layer["offsety"].asFloat());
        }
      }
    }
  }
}

void LevelLoader::load(Level& level) {
  for (auto iter : subworlds) {
    std::size_t index = iter.first;
    auto& subworld_data = iter.second;

    if (not level.subworldExists(index)) {
      level.createSubworld(index);
    }

    Subworld& subworld = level.getSubworld(index);
    subworld.setBounds(subworld_data.bounds);
    subworld.setTheme(subworld_data.theme);
    subworld.setEntities(subworld_data.entities);
    subworld.setTilemap(subworld_data.tilemap);
    subworld.setWaterHeight(subworld_data.water_height);
  }
}
}
