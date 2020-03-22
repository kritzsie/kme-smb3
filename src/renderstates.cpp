#include "renderstates.hpp"

#include <ctgmath>

namespace kme {
RenderFrame::RenderFrame(
std::string texture, Rect<int> cliprect, Vec2f offset, float duration)
: texture(texture), cliprect(cliprect), offset(offset), duration(duration) {}

// begin RenderStates
RenderStates::RenderStates() : state("idle"), offset(0) {}

void RenderStates::pushFrame(std::string state_arg, RenderFrame renderframe) {
  states.at(state_arg).push_back(renderframe);
}

void RenderStates::pushFrame(std::string state_arg, std::string texture,
                             Rect<int> cliprect, Vec2f offset_arg, float duration) {
  states[state_arg].push_back(
    RenderFrame{texture, cliprect, offset_arg, duration}
  );
}

void RenderStates::pushFrame(std::string state_arg, std::string texture,
                             Vec2i origin, Vec2f offset_arg, float duration) {
  pushFrame(state_arg, texture, Rect<int>(origin, Vec2i(16, 16)), offset_arg, duration);
}

StringList RenderStates::getStateList() const {
  StringList state_list;

  state_list.reserve(states.size());
  for (auto& it : states) {
    state_list.push_back(it.first);
  }

  return state_list;
}

std::size_t RenderStates::getFrameCount(std::string state_arg) const {
  return states.at(state_arg).size();
}

std::size_t RenderStates::getFrameOffset(float time) const {
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

void RenderStates::setState(std::string state_arg, std::size_t offset_arg) {
  state = state_arg;
  offset = offset_arg;
}

void RenderStates::setState(std::string state_arg) {
  setState(state_arg, 0);
}

const std::string& RenderStates::getState() const {
  return state;
}

const RenderFrame& RenderStates::getFrame() const {
  return states.at(state).at(offset);
}
// end RenderStates
}
