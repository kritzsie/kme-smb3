#pragma once

#include "../../renderstates.hpp"
#include "../../types.hpp"

#include <string>

namespace kme {
struct Background {
public:
  void registerBackground(std::string name, RenderFrames background);
  const RenderFrames& getBackground(std::string name) const;

private:
  StringTable<RenderFrames> backgrounds;
};
}
