#pragma once

#include <string>

namespace kme {
enum class Powerup {
  NONE, MUSHROOM, FIRE, LEAF, FROG, TANUKI, HAMMER
};

std::string getPowerupName(Powerup powerup);
}
