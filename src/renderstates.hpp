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

class RenderStates {
public:
  RenderStates();

  void pushFrame(std::string, RenderFrame);
  void pushFrame(std::string, std::string, Vec2i, Vec2f, float);
  void pushFrame(std::string, std::string, Rect<int>, Vec2f, float);

  void setState(std::string);
  void setState(std::string, std::size_t);

  StringList getStateList() const;

  std::size_t getFrameCount(std::string state_arg) const;
  std::size_t getFrameOffset(float time) const;

  const std::string& getState() const;
  const RenderFrame& getFrame() const;

private:
  std::string state;
  std::size_t offset;
  std::map<std::string, RenderFrames> states;
};
}
