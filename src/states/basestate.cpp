#include "basestate.hpp"

namespace kme {
BaseState::BaseState(BaseState* parent, Engine* engine) : parent(parent), engine(engine) {}
}
