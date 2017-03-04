#ifndef _BRIDGE_WORSTSEARCHINCLUDED
#define _BRIDGE_WORSTSEARCHINCLUDED

/*
 *   Copyright (C) 2009 by Paul Bethe   *
 *   pmb309@cs.nyu.edu   *
 *
 * Worst Case Search
 * class
 ***************************************************************************/

#include <memory>
#include <ostream>
#include <vector>

#include "dtac/BranchingStats.h"
#include "dtac/Claim.h"
#include "dtac/CombinedDefender.h"
#include "dtac/Hand.h"
#include "dtac/HandCache.h"

namespace dtac {

/** to pool SPs of stuff */
template <typename T>
class Pool {
 public:
  typedef std::shared_ptr<T> TypeSP;
  TypeSP getFromPool() {
    if (!arrayPool.empty()) {
      TypeSP res = arrayPool.back();
      arrayPool.pop_back();
      return res;
    } else {  // just increase...
      return TypeSP(new T);
    }
  }

  void reserve(int num) {
    arrayPool.reserve(num);
    for (int i = 0; i < num; ++i) arrayPool.push_back(TypeSP(new T));
  }

  void returnToPool(TypeSP inp) { arrayPool.push_back(inp); }

 protected:
  std::vector<TypeSP> arrayPool;
};

#define PlayArraySP std::shared_ptr<std::vector<Play> >

class WorstSearch {
 public:
  /* hand of player on lead, their partner, and info on all cards gone
  */
  WorstSearch(const Hand& origLead, const Hand& partner, const Hand& played,
              SUIT trumps, bool claimK = true);
  WorstSearch();

  std::shared_ptr<Claim> search(const CombinedDefender* constraints = NULL);
  std::shared_ptr<Claim> search(bool oppsLead, SUIT oppsSuit = NOTRUMP);
  std::shared_ptr<Claim> search(int toTake,
                                const CombinedDefender* constraints = NULL,
                                bool oppsLead = false, SUIT oppsSuit = NOTRUMP);
  std::shared_ptr<Claim> searchRequire(SUIT requireFirstRound);
  std::shared_ptr<Claim> searchRequire(int toTake, SUIT requireFirstRound);

  // reset using orig hands, to benefit from
  void reset(const Hand& origLead, const Hand& partner, const Hand& played,
             SUIT newTrumps = NOTRUMP, bool resetCache = false);

  Hand origLead, partner, played;
  //  ClaimSP result;
  CombinedDefender defender;
  SUIT trumps;

  // stat keeping
  void printCacheStats(std::ostream& os) const;
  std::shared_ptr<CStats> getCacheStats() const;
  BranchingStats bstats;

 protected: /* methods */
  bool accept(const ClaimTrick& trick, const Claim& claim) const;
  void applyPlay(const ClaimTrick& trick, Claim& claim);
  void undoPlay(Claim& claim);

  // for claim-K
  bool claimAfterLoss(const ClaimTrick& trick, LoseATrick& tryK,
                      const Claim& currClaim) const;
  // only call searchAfterLoss from inside claimAfterLoss
  bool searchAfterLoss(const CombinedDefender& adjustedDfndr,
                       const ClaimTrick& trick, LoseATrick& tryK,
                       const Claim& currClaim,
                       std::vector<std::shared_ptr<Claim> >& sClaims) const;

  void initDefender();
  bool findSolution(SUIT suit, bool partnerLeads, bool oppsLead = false);

  template <bool claimK>
  bool findSolutionFast(SUIT suit, bool partnerLeads, bool oppsLead = false);

 protected:
  HandCache cache;
  std::shared_ptr<Claim> result;

  void returnToPool(PlayArraySP inp, PlayArraySP inp2);
  Pool<std::vector<Play> > playArrayPool;
  Pool<LoseATrick> loseTrickPool;
  bool searched;
  int losers;

 private:
  WorstSearch(const WorstSearch& other);
  const bool ALLOW_CLAIM_K;
};

}  // namespace dtac

#endif /* inc */
