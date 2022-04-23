#include "states.hpp"

#include <string>

namespace kme {
std::string getStateName(EState state) {
  switch (state) {
  default:
  case EState::IDLE:
    return "IDLE";
  case EState::DEAD:
    return "DEATH";
  case EState::WALK:
    return "WALK";
  case EState::RUN:
    return "RUN";
  case EState::SLIP:
    return "SLIP";
  case EState::DUCK:
    return "DUCK";
  case EState::AIRBORNE:
    return "JUMP";
  case EState::RUNJUMP:
    return "RUNJUMP";
  case EState::UNDERWATER:
    return "UNDERWATER";
  case EState::SWIM:
    return "SWIM";
  case EState::DOOR:
    return "DOOR";
  case EState::PIPE:
    return "PIPE";
  }
}
}
