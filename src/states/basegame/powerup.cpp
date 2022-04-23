#include "powerup.hpp"

namespace kme {
std::string getPowerupName(Powerup powerup) {
  switch (powerup) {
  case Powerup::MUSHROOM:
    return "BIG";
    break;
  case Powerup::FIRE:
    return "FIRE";
    break;
  case Powerup::LEAF:
    return "RACCOON";
    break;
  case Powerup::FROG:
    return "FROG";
    break;
  case Powerup::TANUKI:
    return "TANUKI";
    break;
  case Powerup::HAMMER:
    return "HAMMER";
    break;
  default:
    return "";
    break;
  }
}
}
