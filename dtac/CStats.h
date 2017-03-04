#ifndef _BRIDGE_CSTATS_H_
#define _BRIDGE_CSTATS_H_

#include <ostream>

/**
 * Helper class for stat collection.
 */
namespace dtac {

class CStats {
 public:
  CStats() : collisions(0), hits(0), misses(0) {}
  int collisions, hits, misses;
  void mergeIn(const CStats& other) {
    collisions += other.collisions;
    hits += other.hits;
    misses += other.misses;
  }
  void print(std::ostream& os) const;
};

}  // namespace dtac

#endif
