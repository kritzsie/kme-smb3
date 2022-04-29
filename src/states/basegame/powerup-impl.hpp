#pragma once

#include <string_view>

namespace kme {
constexpr std::string_view getPowerupName(Powerup powerup) {
  switch (powerup) {
    default:                return "";
    case Powerup::MUSHROOM: return "BIG";
    case Powerup::FIRE:     return "FIRE";
    case Powerup::LEAF:     return "RACCOON";
    case Powerup::FROG:     return "FROG";
    case Powerup::TANUKI:   return "TANUKI";
    case Powerup::HAMMER:   return "HAMMER";
  }
}

constexpr std::size_t getPowerupTier(Powerup powerup) {
  switch (powerup) {
  case Powerup::NONE:     return 0;
  case Powerup::MUSHROOM: return 1;
  default:                return 2;
  }
}
}
