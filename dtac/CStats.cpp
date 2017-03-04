#include "dtac/CStats.h"

using std::endl;
using std::ostream;

namespace dtac {

void CStats::print(ostream& os) const {
  if (hits > 0 || misses > 0) {
    os << "Cache statistics: coll=" << collisions << " hits=" << hits
       << " miss=" << misses << " hit rate=" << ((double)hits) / (hits + misses)
       << endl;
  } else {
    os << "Cache empty." << endl;
  }
}

}  // namespace dtac
