#ifndef _BRIDGE_BRANCHING_STATS_H_
#define _BRIDGE_BRANCHING_STATS_H_

#include <iostream>

namespace dtac {

class BranchingStats {
  int branches, internals;

 public:
  BranchingStats() : branches(0), internals(0) {}

  void reset();
  void print(std::ostream& os) const;
  void mergeIn(const BranchingStats& other);
  // add # branches taken from node
  void addNode(int branchesTaken);
};

}  // end namespace dtac
#endif /* _BRIDGE_BRANCHING_STATS_H_ */
