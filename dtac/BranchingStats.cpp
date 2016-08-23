#include "dtac/BranchingStats.hpp"

using std::endl;
using std::ostream;

namespace dtac {

void BranchingStats::print(ostream& os) const {
  if (branches == 0) return;
  os << "Branching factor = " << ((double)branches) / internals << " ("
     << branches << "/" << internals << ")." << endl;
}

void BranchingStats::mergeIn(const BranchingStats& other) {
  branches += other.branches;
  internals += other.internals;
}

// add # branches taken from node
void BranchingStats::addNode(int branchesTaken) {
  ++internals;
  branches += branchesTaken;
}

void BranchingStats::reset() {
  branches = 0;
  internals = 0;
}
}  // end namespace dtac
