#pragma once

#include <string>

namespace kme {
constexpr std::string_view getStateName(EState state) {
  switch (state) {
    case EState::IDLE: default: return "IDLE";
    case EState::DEAD:          return "DEATH";
    case EState::WALK:          return "WALK";
    case EState::RUN:           return "RUN";
    case EState::SLIP:          return "SLIP";
    case EState::DUCK:          return "DUCK";
    case EState::AIRBORNE:      return "JUMP";
    case EState::RUNJUMP:       return "RUNJUMP";
    case EState::UNDERWATER:    return "UNDERWATER";
    case EState::SWIM:          return "SWIM";
    case EState::DOOR:          return "DOOR";
    case EState::PIPE:          return "PIPE";
  }
}
}
