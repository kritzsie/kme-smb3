#pragma once

#include "geometry/rect.hpp"
#include "geometry/vec2.hpp"
#include "types.hpp"

#include <string>
#include <vector>

namespace kme {
using RenderFrames = std::vector<struct RenderFrame>;

struct RenderFrame {
  std::string texture;
  Rect<int> cliprect;
  Vec2f offset;
  float duration;

  RenderFrame(std::string texture, Rect<int> cliprect, Vec2f offset, float duration);
};

class RenderState {
public:
  RenderState();
  RenderState(std::string label);

  void setState(std::string label);
  void setState(std::string label, std::size_t offset);

  std::string getLabel() const;
  std::size_t getOffset() const;

private:
  std::string label;
  std::size_t offset;
};

class RenderStates {
public:
  RenderStates();

  void pushFrame(std::string, RenderFrame);
  void pushFrame(std::string, std::string, Vec2i, Vec2f, float);
  void pushFrame(std::string, std::string, Rect<int>, Vec2f, float);

  StringList getStateList() const;

  std::size_t getFrameCount(std::string state_arg) const;
  std::size_t getFrameOffset(std::string label, float time) const;

  const RenderFrame& getFrame(const RenderState& label) const;
  const RenderFrame& getFrame(std::string label, std::size_t offset) const;

private:
  StringTable<RenderFrames> states;
};
}
