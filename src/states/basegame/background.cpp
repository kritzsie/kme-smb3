#include "background.hpp"

#include "../../renderstates.hpp"

#include <string>

namespace kme {
void Background::registerBackground(std::string name, RenderFrames background) {
  backgrounds.emplace(name, std::move(background));
}

const RenderFrames& Background::getBackground(std::string name) const {
  return backgrounds.at(name);
}
}
