#pragma once

#include <string_view>

namespace kme {
enum class Powerup {
  NONE, MUSHROOM, FIRE, LEAF, FROG, TANUKI, HAMMER
};

constexpr std::string_view getPowerupName(Powerup powerup);
}
