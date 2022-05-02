#include "background.hpp"

#include "../../renderstates.hpp"

#include <memory>
#include <string>

namespace kme {
void Background::registerBackground(std::string name, RenderFrames background) {
  backgrounds.emplace(name, std::make_shared<RenderFrames>(std::move(background)));
}

std::shared_ptr<const RenderFrames> Background::getBackground(std::string name) const {
  return backgrounds.at(name);
}
}
