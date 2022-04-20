#include "map_codec.hpp"

#include "states/basegame/tilemap.hpp"
#include "util.hpp"

#include <json/reader.h>

#include <vector>

namespace kme {
MapLoader::MapLoader(std::string name) {
  std::stringstream ss;
  ss << "/maps/" << name << ".tmj";

  std::vector<char> data = util::readFile(ss.str());
  Json::Reader reader;
  if (reader.parse(data.data(), root)) {
    bounds.width = root["width"].asInt();
    bounds.height = root["height"].asInt();
  }
  else {
    // TODO: throw an appropriate exception here
    throw std::runtime_error("map failed to load");
  }
}

Rect<int> MapLoader::getBounds() {
  return bounds;
}

void MapLoader::loadTiles(Tilemap& tiles) {
  for (auto it : root["layers"]) {
    if (it["name"].asString() == "Tile Layer 1") {
      auto data = util::base64_decode(it["data"].asString());
      for (std::size_t i = 0; i < data.size(); i += 4) {
        std::size_t id = static_cast<UInt8>(data[i]);
        id += static_cast<UInt8>(data[i + 1]) << 8;
        id += static_cast<UInt8>(data[i + 2]) << 16;
        id += static_cast<UInt8>(data[i + 3]) << 24;
      }
      break;
    }
  }
}
}
