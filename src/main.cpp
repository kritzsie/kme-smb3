#include "engine.hpp"

int main(int argc, char** argv) {
  kme::Engine engine(argc, argv);
  return engine.exec();
}
