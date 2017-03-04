#include "dtac/Play.h"

namespace dtac {

std::string Play::toString() const {
  return std::string("") + suit2Char(suit) + rank2Char(rank);
}

std::ostream& operator<<(std::ostream& os, const Play& p) {
  os << p.toString();
  return os;
}

}  // namespace dtac
