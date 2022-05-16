#include "gameloader.hpp"

#include "../../math.hpp"
#include "../../types.hpp"
#include "../../util.hpp"
#include "tiledefs.hpp"

#include <json/reader.h>
#include <json/value.h>

#include <sstream>
#include <string>

#include <cstddef>
#include<iostream>
namespace kme {
const StringTable<TileDef::CollisionType> TileDefLoader::collision_table = {
  {"none", TileDef::CollisionType::NONE},
  {"solid", TileDef::CollisionType::SOLID},
  {"platform", TileDef::CollisionType::PLATFORM},
  {"slope", TileDef::CollisionType::SLOPE}
};

TileDefLoader::TileDefLoader(std::string filename) {
  Json::Value root;
  Json::Reader reader;
  if (reader.parse(util::readFile(filename).data(), root)) {
    for (const auto& val : root["tiles"]) {
      std::string name = val["name"].asString();
      TileDef::CollisionType collision = collision_table.at(
        val["collision"].isNull() ? "none" : val["collision"].asString()
      );
      std::string type = \
        val["type"].isNull() ? "single" : val["type"].asString();
      if (type == "indexed") {
        std::size_t offset = val["firstindex"].asUInt();
        for (auto iter = val["frames"].begin(); iter != val["frames"].end(); ++iter) {
          std::size_t index = iter.index() + offset;
          const auto& frame = *iter;
          std::stringstream ss;
          ss << name << "_" << index;
          std::string texture = frame["texture"].asString();
          Vec2i origin(frame["origin"][0].asInt(), frame["origin"][1].asInt());
          tile_data[ss.str()].pushFrame(texture, origin, 0.f);
          tile_data[ss.str()].setCollisionType(collision);
        }
      }
      else if (type == "animated") {
        for (const auto& frame : val["frames"]) {
          std::string texture = frame["texture"].asString();
          Vec2i origin(frame["origin"][0].asInt(), frame["origin"][1].asInt());
          tile_data[name].pushFrame(texture, origin, 8.f / 60.f);
          tile_data[name].setCollisionType(collision);
        }
      }
      else if (type == "single") {
        const auto& frame = val["frames"][0];
        std::string texture = frame["texture"].asString();
        Vec2i origin(frame["origin"][0].asInt(), frame["origin"][1].asInt());
        tile_data[name].pushFrame(texture, origin, 0.f);
        tile_data[name].setCollisionType(collision);
      }
    }
  }
}

void TileDefLoader::load(TileDefs& tiledefs) {
  for (auto i : tile_data) {
    tiledefs.registerTileDef(i.first, i.second);
  }
}
}
