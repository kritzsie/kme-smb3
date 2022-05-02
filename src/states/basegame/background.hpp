#pragma once

#include "../../renderstates.hpp"
#include "../../types.hpp"

#include <memory>
#include <string>

namespace kme {
struct Background {
public:
  void registerBackground(std::string name, RenderFrames background);
  std::shared_ptr<const RenderFrames> getBackground(std::string name) const;

private:
  StringTable<std::shared_ptr<RenderFrames>> backgrounds;
};
}
