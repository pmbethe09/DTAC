/***************************************************************************
 *   Copyright (C) 2009 by Paul Bethe   *
 *   pmb309@cs.nyu.edu   *
 *
 * Worst Case Search
 * the opponents play the best card they have on every possilbe round of
 * play in that suit
 * eg:  we have KQxx opposite ATx -
 * SO: opponents hold J 6th... play the Jack for up to 6 rounds. *
 * Will discover to play A K Q in separate plays - only 3 winners.
 ***************************************************************************/
#include "dtac/WorstSearch.hpp"

#include <memory>

#include "dtac/SuitState.hpp"

using std::endl;
using std::ostream;
using std::vector;

namespace dtac {

#define BRANCH_STATS_ON true

#define ALLOW_N_LOSERS 1
#define ALLOW_LOSERS_AFTER_FIRST true
//#define ALLOW_CLAIM_K true

#define LoseATrickSP std::shared_ptr<LoseATrick>
#define ClaimSP std::shared_ptr<Claim>

WorstSearch::WorstSearch()
    : trumps(NUM_SUITS),
      cache(Hand(), Hand()),
      searched(true /*make unusable*/),
      losers(ALLOW_N_LOSERS),
      ALLOW_CLAIM_K(true) {}

WorstSearch::WorstSearch(const Hand& origLead, const Hand& partner,
                         const Hand& played, SUIT trumps, bool claimK)
    : origLead(origLead),
      partner(partner),
      played(played),
      trumps(trumps),
      cache(origLead, partner),
      searched(false),
      losers(claimK ? ALLOW_N_LOSERS : 0),
      ALLOW_CLAIM_K(claimK) {
  /// create pool of plays to search through
  const int worst_expect = 26;
  playArrayPool.reserve(worst_expect);
}

/* as per paper, 'accept' means the CD cannot beat this */
inline bool WorstSearch::accept(const ClaimTrick& trick,
                                const Claim& claim) const {
  return trick.beats(claim, defender, trumps);
}

/** apply play to claim assume caller knows if this is a win, or just a duck */
inline void WorstSearch::applyPlay(const ClaimTrick& trick, Claim& claim) {
  const Play& lead = trick.lead;
  const Play& follow = trick.follow;
  bool pdLeads = trick.pdLeads;

  claim.round_of[(int)trick.suitLead] += 1;
  // update hands w/ masks
  claim.tricks.push_back(trick);
  int sum = 0;
  for (int i = 0; i < NUM_SUITS; ++i) sum += claim.round_of[i];
  ASSERT_MSG(claim.tricks.size() == sum, "round of matches num claim tricks");

  if (!pdLeads) {
    ASSERT_MSG_DBG(origLead.hasCard(lead.suit, lead.rank), "has card");
    ASSERT_MSG_DBG(partner.hasCard(follow.suit, follow.rank), "has card");
    lead.play(origLead);
    follow.play(partner);
  } else {
    ASSERT_MSG_DBG(partner.hasCard(lead.suit, lead.rank), "has card");
    ASSERT_MSG_DBG(origLead.hasCard(follow.suit, follow.rank), "has card");
    lead.play(partner);
    follow.play(origLead);
  }
  // these 2 strange looking lines ADD the lead and follow to the played deck
  lead.unplay(played);
  follow.unplay(played);
}

/** return claim to state prior to applyPlay */
inline void WorstSearch::undoPlay(Claim& claim) {
  ClaimTrick& toUndo = claim.tricks[claim.tricks.size() - 1];
  const Play& lead = toUndo.lead;
  const Play& follow = toUndo.follow;
  bool gtZ = claim.round_of[(int)toUndo.suitLead] > 0;
  if (!gtZ) {
    ASSERT_MSG_DBG(gtZ, "can't set round_of to negative");
  }
  claim.round_of[(int)toUndo.suitLead] -= 1;
  if (!toUndo.pdLeads) {
    lead.unplay(origLead);
    follow.unplay(partner);
  } else {
    lead.unplay(partner);
    follow.unplay(origLead);
  }
  // these 2 strange looking lines REMOVE the lead and follow from the played
  // deck
  ASSERT_MSG_DBG(played.hasCard(lead.suit, lead.rank), "have card");
  lead.play(played);
  follow.play(played);

  claim.tricks.resize(claim.tricks.size() - 1);
  int sum = 0;
  for (int i = 0; i < NUM_SUITS; ++i) sum += claim.round_of[i];
  bool match = claim.tricks.size() == sum;
  if (!match) {
    ASSERT_MSG(match, "round of matches num claim tricks");
  }
}

/* cache 'lead', 'follow' SPs, not multi-threaded */
inline void WorstSearch::returnToPool(PlayArraySP inp1, PlayArraySP inp2) {
  playArrayPool.returnToPool(inp1);
  playArrayPool.returnToPool(inp2);
}

// return true if we still can claim after losing this trick
bool WorstSearch::claimAfterLoss(const ClaimTrick& trick, LoseATrick& tryK,
                                 const Claim& currClaim) const {
  // Create new search
  // 1. Opps win cheaply -- assume valid for CD search
  // 2. Opps may trump in.
  bool canWinCheap = false, canRuffIn = false;

  SUIT sLead = trick.lead.suit;
  int sIdx = (int)sLead;
  CombinedDefender adjustedDfndr(defender);
  // update CD with 'round_of' info
  for (int s = 0; s < NUM_SUITS; ++s) {
    adjustedDfndr.length[s] -= currClaim.round_of[s];
    if (adjustedDfndr.length[s] < 0) adjustedDfndr.length[s] = 0;
    adjustedDfndr.min_length[s] -= currClaim.round_of[s];
    if (adjustedDfndr.min_length[s] < 0)  // and floor at 0
      adjustedDfndr.min_length[s] = 0;
  }

  bool hadSuit = false;
  if (adjustedDfndr.length[sIdx] > 0) {
    --adjustedDfndr.length[sIdx];
    hadSuit = true;
  }
  if (adjustedDfndr.min_length[sIdx] > 0) --adjustedDfndr.min_length[sIdx];

  Hand theorDef = Hand::complement(origLead, partner, played);
  if (hadSuit) {  // had cards in that suit, maybe won that way
    // find lowest card in CD that beats trick
    SUIT s = trick.winner->suit;
    RANK r = trick.winner->winCheaply(theorDef);
    if (r <= ACE) {
      // remove the cheapest play, and reset the highest card
      canWinCheap = true;
      theorDef.removeCard(s, r);
      if (s != sLead)  // ruffing
        --adjustedDfndr.length[s];
      if (adjustedDfndr.length[s] > 0)
        adjustedDfndr.highest[(int)s] = theorDef.getHighestInSuit(s);
      if (!searchAfterLoss(adjustedDfndr, trick, tryK, currClaim,
                           tryK.winSuitClaims))
        return false;
    }
  }

  // Also consider they could ruffIn
  bool oppsCouldRuffIn = trumps != NOTRUMP &&
                         adjustedDfndr.length[trumps] > 0 &&
                         trick.lead.suit != trumps;
  if (oppsCouldRuffIn) {
    if (trick.winner->suit != trumps) {  // simple cheap ruff in
      canRuffIn = true;
      if (adjustedDfndr.min_length[trumps] >
          0)  // assume 'short' hand ruffs in (Worst Case of course)
        --adjustedDfndr.min_length[trumps];
      else
        --adjustedDfndr.length[trumps];
      return searchAfterLoss(adjustedDfndr, trick, tryK, currClaim,
                             tryK.ruffSuitClaims);
    } else {  // maybe we can overruff with a higher trump?
      if (adjustedDfndr.highest[trumps] > trick.winner->rank) {
        SUIT s = trick.winner->suit;
        RANK r = trick.winner->winCheaply(theorDef);
        if (r <= ACE) {
          // can overruff
          canRuffIn = true;
          theorDef.removeCard(s, r);
          --adjustedDfndr.length[trumps];
          return searchAfterLoss(adjustedDfndr, trick, tryK, currClaim,
                                 tryK.ruffSuitClaims);
        }
      }
    }  // end else overruf
  }    // end else ruff

  ASSERT_MSG(canWinCheap || canRuffIn,
             "can't apply Loser if they don't win");  // else, how could we lose
                                                      // the trick!
  return true;
}

// called only from claimAfterLoss
bool WorstSearch::searchAfterLoss(const CombinedDefender& adjustedDfndr,
                                  const ClaimTrick& trick, LoseATrick& tryK,
                                  const Claim& currClaim,
                                  vector<ClaimSP>& sClaims) const {
  const int newTake = currClaim.toTake - currClaim.tricks.size();

  WorstSearch subSearch(origLead, partner, played, trumps, true);
  subSearch.losers = losers - 1;

  // try every suit the opps have
  for (SUIT s = SPADES; (int)s >= 0; --s) {
    if (adjustedDfndr.length[(int)s] > 0) {
      subSearch.reset(origLead, partner, played);
      // apply play, AND adjust CD.
      Claim dummy(1);
      subSearch.applyPlay(trick, dummy);

      ClaimSP c = subSearch.search(newTake, &adjustedDfndr, true /*oppsLead*/,
                                   s /*oppsSuit*/);
      if (!c) {
        return false;  // they beat us with 's' switch
      }
      // else
      sClaims[(int)s] = ClaimSP(new Claim(*c));
    }
  }
  return true;  // all suits with cards returned true
}

bool WorstSearch::findSolution(
    SUIT lastSuit, bool partnerLeads,
    bool oppsLead /*for initial when opps are on lead*/) {
  const int tricksIn = result->tricks.size();
  const bool claimK =
      ALLOW_CLAIM_K && losers > 0 &&
      (result->toTake - tricksIn) < origLead.size() &&
      (tricksIn == 0 || ALLOW_LOSERS_AFTER_FIRST);  // need less than I hold
  if (claimK) {
    // try easy way
    int svLosers =
        losers;  // so that the cache key is correct in 'quick' search
    losers = 0;
    bool simpleOne = findSolutionFast<false>(lastSuit, partnerLeads, oppsLead);
    losers = svLosers;
    if (simpleOne) return simpleOne;

    // losers allowed
    return findSolutionFast<true>(lastSuit, partnerLeads, oppsLead);
  } else
    return findSolutionFast<false>(lastSuit, partnerLeads, oppsLead);
}

/* search for claim, return true if found */
template <bool CLAIM_K>
bool WorstSearch::findSolutionFast(
    SUIT lastSuit, bool partnerLeads,
    bool oppsLead /*for initial when opps are on lead*/) {
  // base-case
  Claim& claim = *result;
  const int tricksIn = claim.tricks.size();
  if (tricksIn == claim.toTake) {
    return true;
  }

  // determine suit lengths
  SuitState ss(*this, claim, partnerLeads, trumps);

  int branchesTaken = 0;
  LoseATrickSP tryKSP;

  bool oppsCouldRuffIn =
      CLAIM_K && trumps != NOTRUMP && defender.length[trumps] > 0;

#ifdef DEBUG
  const Hand leadInvar = origLead, follInvar = partner;
#endif

  PlayArraySP leadsSP = playArrayPool.getFromPool(),
              follsSP = playArrayPool.getFromPool();
  vector<Play> &leads = *leadsSP, &follows = *follsSP;
  const int suit_iters = !oppsLead ? NUM_SUITS : 1;
  for (int s = 0; s < suit_iters; ++s) {
    int suitIdx = !oppsLead ? (int)ss.ordering[s] : lastSuit;
    SUIT suit = (SUIT)suitIdx;

    if (suit == NUM_SUITS) return false;
    if (ss.lengths[suitIdx] == 0 &&
        !oppsLead)  // no more to look at, was sorted
      continue;

    ss.nextPlaysInSuit(this->played, suit, partnerLeads, leads, follows,
                       oppsLead);

    if (leads.size() == 0)  // leader has none of these
      continue;

    // if claimAll, assume opponent does a snatch and beat me
    // else, allow them to duck and explore
    const bool couldDuck =
        CLAIM_K && (defender.length[suit] > 1 ||
                    (oppsCouldRuffIn && defender.length[suit] == 0));

    // apply each play, and recurse
    for (int i = 0; i < leads.size(); ++i) {
      const Play& lead = leads[i];
      // SUIT suit; bool pdLeads;

      for (int f = 0; f < follows.size(); ++f) {
        const Play& follow = follows[f];

        ClaimTrick trick(lead, follow, trumps, partnerLeads,
                         oppsLead ? suit : NOTRUMP);
        bool wins = accept(trick, claim);

        bool duckWorks = false;  // change to true, if we claim when opp ducks

        if (wins || couldDuck) {
          /* applyPlay updates theState */
          applyPlay(trick, claim);

          // exit when done.
          if (wins && tricksIn + 1 == claim.toTake) {
            returnToPool(leadsSP, follsSP);
            return true;
          }

          bool nextPdLeads = !trick.leadWins;
          // follow.beats(lead, trumps);
          if (partnerLeads) nextPdLeads = !nextPdLeads;

          HandCache::cacheType::value_type cacheKey =
              cache.getCacheKey(origLead, partner, losers, claim, nextPdLeads);

          bool notYet = cache.notInCache(cacheKey);
          /*if (cacheKey.first.first == 13528391089132070) {
            // FIXME - hits cache at club to K
            cerr << "hmmm?";
            if (notYet)
              cerr << "OK, but?" << endl;
            else
              cerr << "What?" << endl;
              }*/

          bool found = false;
          ClaimSP prev;
          if (notYet) {
            // add to cache, then recurse
            cache.addToCache(cacheKey);
            if (BRANCH_STATS_ON) ++branchesTaken;
            found = findSolution(suit, nextPdLeads, false);
            // findSolutionFast<CLAIM_K>(suit, nextPdLeads, false);
          } else {
            prev = cache.getSolution(cacheKey);
            found = prev.get();
          }

          if (found) {
            if (notYet) {
              if (BRANCH_STATS_ON) bstats.addNode(branchesTaken);
              cache.addSolution(cacheKey, result);
            }
            // return to pool, just in case reuse WS

            if (wins) {  // CD could never beat
              returnToPool(leadsSP, follsSP);
              return true;
            }
            duckWorks = true;
            if (notYet) {
              // copy this claim, as we are about to 'undo'
              tryKSP = loseTrickPool.getFromPool();
              LoseATrick& tryK = *tryKSP;
              tryK.round = tricksIn + 1;
              // FIXME
              tryK.setDuckLine(claim);  // copy claim from this trick on
              // also fix tricks back
              for (int i = tryK.round, oldSize = claim.tricks.size();
                   i < oldSize; ++i) {
                undoPlay(claim);  // undo tricks past here
              }

              // which includes 'losses'
              if (claim.lostTrick) {
                // might have a lose a trick from the sub-claim
                // and that is now copied into tryK
                claim.lostTrick = LoseATrickSP();
              }
            }
          }

          // even if in cache undo claim.
          undoPlay(claim);
#ifdef DEBUG
          bool wayOutUnch = leadInvar == origLead && follInvar == partner;
          if (!wayOutUnch) {
            ASSERT_MSG(
                leadInvar == origLead && follInvar == partner,
                "invariant holds that hands are the same on the way out");
          }
#endif
        }  // end if wins

        // this play was not a winning play, explore losing a trick
        if (CLAIM_K && !wins && (!couldDuck || duckWorks)) {
          // lose a trick
          if (!tryKSP)  // maybe no duck?
            tryKSP = loseTrickPool.getFromPool();
          LoseATrick& tryK = *tryKSP;
          /* if opp could duck we already explored above */
          if (claimAfterLoss(trick, tryK, claim)) {
            applyPlay(trick, claim);  // reapply for solution
                                      // add to soln
            // claim.tricks.push_back(trick);
            claim.lostTrick = tryKSP;
            returnToPool(leadsSP, follsSP);
            return true;
          } else {
            // failed - return to pool
            tryKSP->reset();
            ASSERT_MSG(!claim.lostTrick, "shouldn't be set");
            loseTrickPool.returnToPool(tryKSP);
          }
        }  // end if K try to lose

      }  // end for each follow
    }    // end for each lead
  }      // end for each suit
  // none found
  if (BRANCH_STATS_ON && branchesTaken > 0) bstats.addNode(branchesTaken);

#ifdef DEBUG
  bool wayOutUnch = leadInvar == origLead && follInvar == partner;
  if (!wayOutUnch) {
    ASSERT_MSG(leadInvar == origLead && follInvar == partner,
               "invariant holds that hands are the same on the way out");
  }
#endif

  returnToPool(leadsSP, follsSP);
  return false;
}

void WorstSearch::initDefender() {
  defender.init(this->origLead, this->partner, this->played);
}

//
ClaimSP WorstSearch::search(bool oppsLead, SUIT oppsSuit) {
  return search(origLead.size(), NULL, oppsLead, oppsSuit);
}

void WorstSearch::reset(const Hand& origLead, const Hand& partner,
                        const Hand& played, SUIT newTrumps, bool resetCache) {
  // if newTrumps == NOTR, then the assumption, is that these are the same 3
  // hands from orig
  // else all will break.
  this->origLead = origLead;
  this->partner = partner;
  this->played = played;
  searched = false;
  if (newTrumps != NOTRUMP) trumps = newTrumps;
  if (resetCache) cache.reset();
}

ClaimSP WorstSearch::search(int toTake, const CombinedDefender* constraints,
                            bool oppsLead, SUIT oppsSuit) {
  //
  // idea: treat 'DISCARD' as a unit play, and do not remove a card from the
  // hand -- then at last trick, fill remaining
  // cards into discards - double check the play was valid end2end.
  // could be invalid for theh case of a squeeze, but should not see those in
  // WCS

  // Trick tricks[13];
  // reset the result object
  ASSERT_MSG(!searched, "not already used");  // already used this object

  if (result)
    result->reset(toTake);
  else
    result = ClaimSP(new Claim(toTake));

  // compute 'Worst' opponent and cache
  initDefender();
  if (constraints) {
    defender.override(*constraints);
  }

  // b-stats reset
  bstats.reset();

  // init cache
  // cache.reset();

  // recursive version
  bool soln = false;
  if (!oppsLead) {  // so we decide what to play!
    soln = findSolution(NUM_SUITS, false);
  } else if (oppsSuit == NOTRUMP) {
    // todo - how to return?
    Claim dummy(toTake);
    // save start state
    Hand origLead2 = origLead, partner2 = partner, played2 = played;

    bool first = true;
    for (SUIT s = SPADES; (int)s >= 0; --s) {
      if (defender.length[s] == 0)
        continue;  // can't lead that suit, dont worry
      if (first) {
        soln = findSolution(s, false, true);
        first = false;
      } else {
        // reset start state
        WorstSearch ws(origLead2, partner2, played2, trumps);
        soln = ws.search(toTake, constraints, true, s).get();
        /* FIXME reuse cache
         * requires 'oppsLead' to be in cache key
         */
        //   origLead = origLead2; partner = partner2; played = played2;
        // soln = findSolution(s, false, true);
      }
      if (!soln) break;
    }
  } else {  // they tabled a lead, so claim for that suit
    soln = findSolution(oppsSuit, false, true);
  }
  searched = true;
  // alternate
  if (soln)
    return result;
  else
    return ClaimSP();
}

ClaimSP WorstSearch::search(const CombinedDefender* constraints) {
  return search(origLead.size(), constraints);
}

void WorstSearch::printCacheStats(ostream& os) const { cache.stats->print(os); }

std::shared_ptr<CStats> WorstSearch::getCacheStats() const {
  return cache.stats;
}
}  // namespace dtac
