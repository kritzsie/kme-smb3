#include "renderstates.hpp"

#include <cmath>

namespace kme {
// begin RenderFrames
void RenderFrames::pushFrame(RenderFrame frame) {
  frames.push_back(frame);
  duration += frame.duration;
}

std::size_t RenderFrames::getFrameCount() const {
  return frames.size();
}

std::size_t RenderFrames::getFrameOffset(float time) const {
  float time_max = 0.f;

  for (const auto& frame : frames) {
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

const RenderFrame& RenderFrames::getFrame(std::size_t offset) const {
  return frames.at(offset);
}
// end RenderFrames

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
  states[label].pushFrame(renderframe);
}

void RenderStates::pushFrame(std::string label, std::string texture,
                             Rect<int> cliprect, Vec2f offset, float duration) {
  pushFrame(label, RenderFrame {
    .texture = texture,
    .cliprect = cliprect,
    .offset = offset,
    .duration = duration
  });
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
  return states.at(label).getFrameCount();
}

std::size_t RenderStates::getFrameOffset(std::string label, float time) const {
  return states.at(label).getFrameOffset(time);
}

const RenderFrame& RenderStates::getFrame(const RenderState& label) const {
  return getFrame(label.getLabel(), label.getOffset());
}

const RenderFrame& RenderStates::getFrame(std::string label, std::size_t offset) const {
  return states.at(label).getFrame(offset);
}
// end RenderStates
}
