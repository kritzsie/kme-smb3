#pragma once

#include "geometry/rect.hpp"
#include "geometry/vec2.hpp"
#include "types.hpp"

#include <string>
#include <vector>

namespace kme {
struct RenderFrame {
  std::string texture;
  Rect<int> cliprect;
  Vec2f offset;
  float duration;
};

class RenderFrames {
public:
  void pushFrame(RenderFrame frame);
  void pushFrame(std::string texture, Rect<int> cliprect,
                 Vec2f offset, float duration);

  std::size_t getFrameCount() const;
  std::size_t getFrameOffset(float time) const;

  const RenderFrame& getFrame(std::size_t offset) const;
  float getDuration() const;

private:
  std::vector<RenderFrame> frames;
  float duration = 0.f;
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

  void pushFrame(std::string label, RenderFrame frame);
  void pushFrame(std::string label, std::string texture, Vec2i origin,
                 Vec2f offset, float duration);
  void pushFrame(std::string label, std::string texture, Rect<int> cliprect,
                 Vec2f offset, float duration);

  StringList getStateList() const;

  std::size_t getFrameCount(std::string state_arg) const;
  std::size_t getFrameOffset(std::string label, float time) const;

  const RenderFrame& getFrame(const RenderState& label) const;
  const RenderFrame& getFrame(std::string label, std::size_t offset) const;

private:
  StringTable<RenderFrames> states;
};
}
