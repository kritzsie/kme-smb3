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
}
