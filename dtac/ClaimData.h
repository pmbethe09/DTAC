#ifndef _CLAIMDATA_HPP
#define _CLAIMDATA_HPP

/** verify claim
 */
#include "dtac/PBNDeal.h"
#include "dtac/Trick.h"

#include <math.h>
#include <ostream>
#include <string>
#include <vector>

namespace dtac {

// to keep stats
template <typename T>
class SumValue {
 public:
  T total, max;
  T totalSqd;
  int samples;
  SumValue() : total(0), max(0), totalSqd(0), samples(0) {}

  void add(T value) {
    ++samples;
    total += value;
    totalSqd += (value * value);
    if (value > max) max = value;
  }

  void printAvgStd(std::ostream& os) const {
    if (samples > 0) {
      double avg = ((double)total) / samples;
      double stddev = sqrt((totalSqd / samples) - avg * avg);
      os << " Avg: " << avg << " StdDev: " << stddev << " Max: " << max;
    } else {
      os << "No data.";
    }
  }
};

class SingleClaim {
 public:
  bool tookAll; /* true if roundsInclaim == tricksTakenInclaim */
  int roundsInclaim, tricksTakenInclaim;  // record of what happened
  Hand declarer, LHO, dummy, RHO;         /* hands - state as of last play */
  SumValue<int> *onLead, *oppsLead;       // can be null, otherwise record stats
  DIRECTION nextToLead;
  SUIT partialLead; /* NOTRUMP, OR, if the nextToLead played a card, then a
                       claim happened -- this has the suit lead */

  void reset() {
    roundsInclaim = 0;
    tricksTakenInclaim = 0;
    tookAll = false;
  }

  SingleClaim(SumValue<int>* onLead, SumValue<int>* oppsLead)
      : onLead(onLead), oppsLead(oppsLead) {
    reset();
  }

  void calcClaim(const PBNDeal& deal, bool printOut);

 private:
  void setHands(const PBNDeal& deal, DIRECTION decl);
  void recordTrick(const std::vector<std::string>& data, int offset);
  SingleClaim(const SingleClaim& other);             // disabled
  SingleClaim& operator=(const SingleClaim& other);  // disabled
};

class ClaimData {
 public:
  SumValue<int> onLead, oppsLead;
  SingleClaim lastHand; /* keep last hand info, then overwrite */

  bool printOut;

  ClaimData() : lastHand(&onLead, &oppsLead), printOut(false) {}
  void calcClaim(const PBNDeal& deal) { lastHand.calcClaim(deal, printOut); }

 private:
  ClaimData& operator=(const ClaimData& other);
  ClaimData(const ClaimData& other);
  /* disabled :
                                         onLead(other.onLead),
     oppsLead(other.oppsLead),
                                         lastHand(other.lastHand),
     printOut(other.printOut) {} */
};

}  // namespace dtac

#endif /*.h */
