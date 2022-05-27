#include "gameloader.hpp"

#include "../../math.hpp"
#include "../../types.hpp"
#include "../../util.hpp"
#include "tiledefs.hpp"

#include <json/reader.h>
#include <json/value.h>

#include <sstream>
#include <stdexcept>
#include <string>

#include <cstddef>

namespace kme {
const StringTable<TileDef::CollisionType> TileDefLoader::collision_table = {
  {"none", TileDef::CollisionType::NONE},
  {"nonsolid", TileDef::CollisionType::NONSOLID},
  {"solid", TileDef::CollisionType::SOLID},
  {"platform", TileDef::CollisionType::PLATFORM},
  {"slope", TileDef::CollisionType::SLOPE},
  {"water", TileDef::CollisionType::WATER},
  {"waterfall", TileDef::CollisionType::WATERFALL},
  {"lava", TileDef::CollisionType::LAVA}
};

TileDefLoader::TileDefLoader(std::string filename) {
  Json::Value root;
  Json::Reader reader;
  if (reader.parse(util::readFile(filename).data(), root)) {
    for (const auto& val : root["include"]) {
      TileDefLoader loader(val.asString());
      for (auto iter : loader.tile_data) {
        tile_data[iter.first] = iter.second;
      }
    }

    for (const auto& tile : root["tiles"]) {
      std::string name = tile["name"].asString();
      std::string type = tile["type"].isNull() ? "single" : tile["type"].asString();
      TileDef::CollisionType collision = tile["collision"].isNull() \
      ? TileDef::CollisionType::NONE
      : collision_table.at(tile["collision"].asString());

      if (type == "indexed") {
        std::size_t offset = tile["firstindex"].asUInt();
        for (auto iter = tile["frames"].begin(); iter != tile["frames"].end(); ++iter) {
          std::size_t index = iter.index() + offset;
          const auto& frame = *iter;
          std::stringstream ss;
          ss << name << "_" << index;
          std::string texture = frame["texture"].asString();
          Vec2i origin = frame["origin"].isNull() \
          ? Vec2i(0, 0)
          : Vec2i(frame["origin"][0].asInt(), frame["origin"][1].asInt());

          tile_data[ss.str()].pushFrame(texture, origin, 0.f);
          tile_data[ss.str()].setCollisionType(collision);
        }
      }
      else if (type == "animated") {
        for (const auto& frame : tile["frames"]) {
          std::string texture = frame["texture"].asString();
          Vec2i origin(frame["origin"][0].asInt(), frame["origin"][1].asInt());
          tile_data[name].pushFrame(texture, origin, 8.f / 60.f);
          tile_data[name].setCollisionType(collision);
        }
      }
      else if (type == "single") {
        const auto& frame = tile["frames"][0];
        std::string texture = frame["texture"].asString();
        Vec2i origin(frame["origin"][0].asInt(), frame["origin"][1].asInt());
        tile_data[name].pushFrame(texture, origin, 0.f);
        tile_data[name].setCollisionType(collision);
      }
    }
  }
  else {
    throw std::runtime_error(filename + " " + reader.getFormattedErrorMessages());
  }
}

void TileDefLoader::load(TileDefs& tiledefs) {
  for (auto i : tile_data) {
    tiledefs.registerTileDef(i.first, i.second);
  }
}
}
