#include "util.hpp"

namespace kme {
Sign operator +(const Sign& rhs) {
  return rhs;
}

Sign operator -(const Sign& rhs) {
  return rhs == Sign::PLUS ? Sign::MINUS : Sign::PLUS;
}
}
