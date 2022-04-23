#pragma once

#include <string>

namespace kme {
enum class EState {
  IDLE, DEAD, DOOR, PIPE,
  WALK, RUN, SLIP, DUCK,
  AIRBORNE, RUNJUMP, UNDERWATER, SWIM
};

std::string getStateName(EState state);
}
