#pragma once

#include <string_view>

namespace kme {
enum class EState {
  IDLE, DEAD, DOOR, PIPE,
  WALK, RUN, SLIP, DUCK,
  AIRBORNE, RUNJUMP, UNDERWATER, SWIM
};

constexpr std::string_view getStateName(EState state);
}
