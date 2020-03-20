#include "engine.hpp"

#include <string>
#include <vector>

int main(int argc, char** argv) {
  kme::Engine engine(argc, argv);

  return engine.exec();
}
