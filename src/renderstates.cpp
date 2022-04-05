#include "renderstates.hpp"

#include <cmath>

namespace kme {
// begin RenderFrame
RenderFrame::RenderFrame(
  std::string texture, Rect<int> cliprect, Vec2f offset, float duration
) : texture(texture), cliprect(cliprect), offset(offset), duration(duration) {}
// end RenderFrame

// begin RenderState
RenderState::RenderState() : RenderState("idle", 0) {
}

RenderState::RenderState(std::string state, std::size_t offset)
: state(state), offset(offset) {}

void RenderState::setState(std::string state_new, std::size_t offset_new) {
  state = state_new;
  offset = offset_new;
}

void RenderState::setState(std::string state_new) {
  setState(state_new, 0);
}

const std::string& RenderState::getState() const {
  return state;
}

std::size_t RenderState::getOffset() const {
  return offset;
}
// end RenderState

// begin RenderStates
RenderStates::RenderStates() {}

void RenderStates::pushFrame(std::string state, RenderFrame renderframe) {
  states.at(state).push_back(renderframe);
}

void RenderStates::pushFrame(std::string state, std::string texture,
                             Rect<int> cliprect, Vec2f offset, float duration) {
  states[state].push_back(
    RenderFrame{texture, cliprect, offset, duration}
  );
}

void RenderStates::pushFrame(std::string state, std::string texture,
                             Vec2i origin, Vec2f offset, float duration) {
  pushFrame(state, texture, Rect<int>(origin, Vec2i(16, 16)), offset, duration);
}

StringList RenderStates::getStateList() const {
  StringList state_list;

  state_list.reserve(states.size());
  for (auto& it : states) {
    state_list.push_back(it.first);
  }

  return state_list;
}

std::size_t RenderStates::getFrameCount(std::string state) const {
  return states.at(state).size();
}

std::size_t RenderStates::getFrameOffset(std::string state, float time) const {
  const RenderFrames& frames = states.at(state);
  float time_max = 0.f;

  for (auto frame : frames) {
    time_max += frame.duration;
  }

  float time_mod = std::fmod(time, time_max);
  float accumulator = 0.f;
  std::size_t counter = 0;

  while (time_mod >= accumulator) {
    accumulator += frames[counter++].duration;
  }

  return (counter - 1) % frames.size();
}

const RenderFrame& RenderStates::getFrame(const RenderState& state) const {
  return getFrame(state.getState(), state.getOffset());
}

const RenderFrame& RenderStates::getFrame(std::string state, std::size_t offset) const {
  return states.at(state).at(offset);
}
// end RenderStates
}
