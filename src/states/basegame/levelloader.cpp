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
    std::size_t subworld = std::stoi(filename);
    SubworldData& data = subworld_data[subworld];

    std::unordered_map<std::size_t, TileID> tileids;

    Json::Value root;
    if (reader.parse(util::readFile(util::join({path.str(), filename}, "/")).data(), root)) {
      data.bounds.width = root["width"].asInt();
      data.bounds.height = root["height"].asInt();
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
          tileids[id] = tiles["type"].asString();
        }
      }
    }

    std::size_t layer_count = 0;
    for (const auto& layer : root["layers"]) {
      if (layer["type"] == "tilelayer") {
        layer_count += 1;
      }
    }

    for (const auto& layer : root["layers"]) {
      if (layer["type"] == "tilelayer") {
        layer_count -= 1;
        auto layer_data = util::base64_decode(layer["data"].asString());
        for (std::size_t i = 0; i < layer_data.size(); i += 4) {
          std::size_t id = 0;
          // convert to little-endian
          for (std::size_t j = 0; j < 4; ++j) {
            id += static_cast<UInt8>(layer_data[i + j]) << (j * 8);
          }
          auto it = tileids.find(id);
          int x = (i / 4) % data.bounds.width;
          int y = data.bounds.height - (i / 4) / data.bounds.width - 1;
          if (it != tileids.end()) {
            data.tilemap.setTile(layer_count, x, y, it->second);
          }
        }
      }
    }

    for (const auto& properties : root["properties"]) {
      if (properties["name"] == "theme") {
        data.theme = properties["value"].asString();
      }
    }
  }
}

void LevelLoader::load(Level& level) {
  for (std::pair<std::size_t, const SubworldData&> iter : subworld_data) {
    std::size_t index = iter.first;
    const auto& data = iter.second;

    if (not level.subworldExists(index)) {
      level.createSubworld(index);
    }

    Subworld& subworld = level.getSubworld(index);
    subworld.setBounds(data.bounds);
    subworld.setTheme(data.theme);
    subworld.setTilemap(data.tilemap);
  }
}
}
