#include "maploader.hpp"

#include "states/basegame/tilemap.hpp"
#include "types.hpp"
#include "util.hpp"

#include <json/reader.h>

#include <map>
#include <string>
#include <vector>

namespace kme {
MapLoader::MapLoader(std::string name) {
  std::stringstream ss;
  ss << "/maps/" << name << ".tmj";

  Json::Reader reader;
  if (reader.parse(util::readFile(ss.str()).data(), root)) {
    bounds.width = root["width"].asInt();
    bounds.height = root["height"].asInt();

    for (const auto& it : root["tilesets"]) {
      std::size_t firstgid = it["firstgid"].asInt();
      std::string source = it["source"].asString();
      StringList path = util::split(ss.str(), "/");
      path.pop_back();
      for (const auto& node : util::split(source, "/")) {
        path.push_back(node);
      }
      loadTileIDs(util::join(path, "/"), firstgid);
    }
  }
  else {
    // TODO: throw an appropriate exception here
    throw std::runtime_error("map failed to load");
  }
}

Rect<int> MapLoader::getBounds() const {
  return bounds;
}

void MapLoader::loadTiles(Tilemap& tilemap) {
  for (const auto& it : root["layers"]) {
    if (it["name"].asString() == "Tile Layer 1") {
      auto data = util::base64_decode(it["data"].asString());
      for (std::size_t i = 0; i < data.size(); i += 4) {
        std::size_t id = 0;
        // convert to little-endian
        for (std::size_t j = 0; j < 4; ++j) {
          id += static_cast<UInt8>(data[i + j]) << (j * 8);
        }
        auto it = tileids.find(id);
        int x = (i / 4) % bounds.width;
        int y = bounds.height - (i / 4) / bounds.width - 1;
        if (it != tileids.end()) {
          tilemap[x][y] = it->second;
        }
      }
      break;
    }
  }
}

void MapLoader::loadTileIDs(std::string path, std::size_t offset) {
  Json::Value tileset_root;
  Json::Reader reader;
  if (reader.parse(util::readFile(path).data(), tileset_root)) {
    for (const auto& it : tileset_root["tiles"]) {
      std::size_t id = it["id"].asInt() + offset;
      tileids[id] = it["type"].asString();
    }
  }
}
}
