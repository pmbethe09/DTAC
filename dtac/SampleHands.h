#ifndef _BRIDGE_SAMPLE_HANDS_INCLUDED
#define _BRIDGE_SAMPLE_HANDS_INCLUDED

#include <utility>
#include <vector>

/// for random sampling of hands
#include "dtac/Constraints.h"
#include "dtac/Hand.h"

namespace dtac {

class SampleReport {
 public:
  SampleReport() : total(0), matches(0), randomsNeeded(0) {}
  SampleReport(int total, int matches, long int randoms)
      : total(total), matches(matches), randomsNeeded(randoms) {}
  int total, matches;
  long int randomsNeeded;
  SampleReport& operator+=(const SampleReport& rhs) {
    total += rhs.total;
    matches += rhs.matches;
    randomsNeeded = rhs.randomsNeeded;
    return *this;
  }
};

class SampleHandsBase {
 public:
  static void setSeed(long int seed);
  static Hand getRandom(const Hand& pdHand, int numCards);
  static Hand getConstrainedRandom(
      const Hand& pdHand, const std::vector<const IConstraint*>& constraints,
      int numCards = 13);
  virtual SampleReport sampleNT(long int seed, const Hand& pdHand) const = 0;
  virtual std::pair<double, Hand> getConstrained(const Hand& pdHand,
                                                 const PointRange& pr,
                                                 const IConstraint& shape,
                                                 int numCards = 13) const = 0;
};

class SampleHandsRandom : public SampleHandsBase {
 public:
  virtual SampleReport sampleNT(long int seed, const Hand& pdHand) const;
  virtual std::pair<double, Hand> getConstrained(const Hand& pdHand,
                                                 const PointRange& pr,
                                                 const IConstraint& shape,
                                                 int numCards = 13) const;
};

class SampleHandsErnie : public SampleHandsBase {
 public:
  virtual SampleReport sampleNT(long int seed, const Hand& pdHand) const;
  virtual std::pair<double, Hand> getConstrained(const Hand& pdHand,
                                                 const PointRange& pr,
                                                 const IConstraint& shape,
                                                 int numCards = 13) const;
};

}  // namespace dtac

#endif /*.h */
