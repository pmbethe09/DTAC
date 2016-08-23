#include "dtac/Trick.hpp"

#include <sstream>

using std::ostringstream;

namespace dtac {

std::string Trick::toString() const {
  ostringstream os;
  for (int i = 0; i < size; ++i) {
    os << suits[i] << ranks[i] << ' ';
  }
  return os.str();
}

}  // namespace dtac
