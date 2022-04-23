#include "renderstates.hpp"

#include <cmath>

namespace kme {
// begin RenderFrame
RenderFrame::RenderFrame(
  std::string texture, Rect<int> cliprect, Vec2f offset, float duration
) : texture(texture), cliprect(cliprect), offset(offset), duration(duration) {}
// end RenderFrame

// begin RenderState
RenderState::RenderState() : RenderState("IDLE") {}

RenderState::RenderState(std::string label_arg) {
  label = label_arg;
  offset = 0;
}

void RenderState::setState(std::string label_arg, std::size_t offset_arg) {
  label = label_arg;
  offset = offset_arg;
}

void RenderState::setState(std::string label_arg) {
  setState(label_arg, 0);
}

std::string RenderState::getLabel() const {
  return label;
}

std::size_t RenderState::getOffset() const {
  return offset;
}
// end RenderState

// begin RenderStates
RenderStates::RenderStates() {}

void RenderStates::pushFrame(std::string label, RenderFrame renderframe) {
  states.at(label).push_back(renderframe);
}

void RenderStates::pushFrame(std::string label, std::string texture,
                             Rect<int> cliprect, Vec2f offset, float duration) {
  states[label].push_back(
    RenderFrame{texture, cliprect, offset, duration}
  );
}

void RenderStates::pushFrame(std::string label, std::string texture,
                             Vec2i origin, Vec2f offset, float duration) {
  pushFrame(label, texture, Rect<int>(origin, Vec2i(16, 16)), offset, duration);
}

StringList RenderStates::getStateList() const {
  StringList state_list;

  state_list.reserve(states.size());
  for (auto& it : states) {
    state_list.push_back(it.first);
  }

  return state_list;
}

std::size_t RenderStates::getFrameCount(std::string label) const {
  return states.at(label).size();
}

std::size_t RenderStates::getFrameOffset(std::string label, float time) const {
  const RenderFrames& frames = states.at(label);
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

const RenderFrame& RenderStates::getFrame(const RenderState& label) const {
  return getFrame(label.getLabel(), label.getOffset());
}

const RenderFrame& RenderStates::getFrame(std::string label, std::size_t offset) const {
  return states.at(label).at(offset);
}
// end RenderStates
}
