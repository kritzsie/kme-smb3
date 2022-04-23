#include "powerup.hpp"

#include <string>

namespace kme {
std::string getPowerupName(Powerup powerup) {
  switch (powerup) {
  case Powerup::MUSHROOM:
    return "BIG";
  case Powerup::FIRE:
    return "FIRE";
  case Powerup::LEAF:
    return "RACCOON";
  case Powerup::FROG:
    return "FROG";
  case Powerup::TANUKI:
    return "TANUKI";
  case Powerup::HAMMER:
    return "HAMMER";
  default:
    return "";
  }
}
}
