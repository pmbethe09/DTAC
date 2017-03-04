#include "dtac/BridgeTestBase.hpp"

#include <gtest/gtest.h>

#include "dtac/WorstSearch.hpp"

#define ASSERT_SP(sp) ASSERT_TRUE(sp.get())

#include <sstream>
#include <string>

using std::istringstream;
using std::string;

namespace dtac {

bool BridgeTestBase::hasCard(const Hand& h, const Play& p) {
  return h.hasCard(p.suit, p.rank);
}

//// helper
void BridgeTestBase::applyTricks(const int toApply, const Claim& claim,
                                 bool& stillOnLead, Hand& lead, Hand& follow,
                                 SUIT trumps, CombinedDefender& cd) {
  // apply this many tricks and see if we find shorter guy
  for (int i = 0; i < toApply; ++i) {
    const ClaimTrick& ct = claim.tricks[i];
    if (stillOnLead) {
      bool OK = hasCard(lead, ct.lead) && hasCard(follow, ct.follow);
      if (!OK) {
        ASSERT_TRUE(OK) << lead.toString() << suit2Char(ct.lead.suit)
                        << rank2Char(ct.lead.rank);
      }
      lead.removeCard(ct.lead.suit, ct.lead.rank);
      follow.removeCard(ct.follow.suit, ct.follow.rank);

      stillOnLead = ct.leadWins;
      //! ct.follow.beats(ct.lead, trumps);
    } else {  // follow on lead now
      ASSERT_TRUE(hasCard(follow, ct.lead) && hasCard(lead, ct.follow));
      follow.removeCard(ct.lead.suit, ct.lead.rank);
      lead.removeCard(ct.follow.suit, ct.follow.rank);

      // invert - since 'follow' is the origLeader
      // stillOnLead = ct.follow.beats(ct.lead, trumps);
      stillOnLead = !ct.leadWins;
    }
    // also decrement the CD for rounds played
    int lsIdx = (int)ct.lead.suit;
    if (cd.length[lsIdx] > 0) --cd.length[lsIdx];
    if (cd.min_length[lsIdx] > 0) --cd.min_length[lsIdx];
  }
}

//// this is the take the next K version -- no 'losers' allowed

void BridgeTestBase::workBack(Hand leadin, Hand followin, Hand dfndr,
                              SUIT trumps, const CombinedDefender& baseCd,
                              const Claim& claim, SUIT leadTrickOne) {
  // OK - try to find a claim
  ASSERT_TRUE(!claim.lostTrick) << "no losers allowed";

  const int size = claim.tricks.size();

  ClaimSP csp;
  for (int toGo = 2; toGo <= size; ++toGo) {
    // not sure about undo - just copy
    Hand lead(leadin), follow(followin);
    CombinedDefender cd(baseCd);

    bool stillOnLead = true;  // lead may change hands

    const int toApply = size - toGo;
    // apply them
    applyTricks(toApply, claim, stillOnLead, lead, follow, trumps, cd);

    bool oppsLead = toGo == size && leadTrickOne != NOTRUMP;

    csp =
        runOne(stillOnLead ? lead : follow, stillOnLead ? follow : lead, dfndr,
               trumps, &cd, claim.toTake - toApply, oppsLead, leadTrickOne);
    if (toGo == size) {
      // printState(stillOnLead ? lead : follow, stillOnLead ? follow : lead,
      //           dfndr, SPADES, cd);
      // csp->print(std::cout);
    }
    ASSERT_SP((csp)) << "backtrack '-" << toApply << "' failed";

    // assert matching
    ASSERT_EQ(csp->tricks.size(), (claim.tricks.size() - toApply))
        << "Same number of tricks, " << csp->tricks.size()
        << "!=" << claim.tricks.size() << "-" << toApply;
    int offset = toApply;
    // claim.tricks.size() - csp->tricks.size(); // for shorter guys
    for (int i = 0; i < csp->tricks.size(); ++i) {
      const ClaimTrick& ctOrig = claim.tricks[i + offset];
      const ClaimTrick& ct = csp->tricks[i];
      bool match = ctOrig == ct;
      if (!match) {
        csp->print(std::cout);
        ASSERT_EQ(ctOrig, ct) << "backtrack '-" << toApply << "' trick " << i
                              << " did not match.\n"
                              << ctOrig.toString() << "!=" << ct.toString();
      }
    }

  }  // for toGo
  // print the last one
  // printState(leadin, followin, dfndr, SPADES, baseCd);
  // csp->print(std::cout);
}

void BridgeTestBase::workBack(const char* hands[3], SUIT trumps,
                              const CombinedDefender& baseCd,
                              const Claim& claim) {
  Hand leadin = Hand::fromString(hands[0]),
       followin = Hand::fromString(hands[1]),
       dfndr = Hand::fromString(hands[2]);
  workBack(leadin, followin, dfndr, trumps, baseCd, claim);
}

ClaimSP BridgeTestBase::runOne(const char* lead, const char* follow,
                               const char* dfnd, SUIT trumps,
                               const CombinedDefender* constraints,
                               int tricks) {
  Hand t1, t2, dfndr;
  t1 = Hand::fromString(lead);
  t2 = Hand::fromString(follow);
  dfndr = Hand::fromString(dfnd);

  return runOne(t1, t2, dfndr, trumps, constraints, tricks);
}

ClaimSP BridgeTestBase::runOne(const char* lead,
                               const char* follow, /* assume dfndr is all */
                               SUIT trumps, const CombinedDefender* constraints,
                               int tricks) {
  Hand t1, t2, dfndr;
  t1 = Hand::fromString(lead);
  t2 = Hand::fromString(follow);
  dfndr = Hand::complement(t1, t2, Hand());

  return runOne(t1, t2, dfndr, trumps, constraints, tricks);
}

ClaimSP BridgeTestBase::runOne(const Hand& lead, const Hand& follow,
                               const Hand& dfndr, SUIT trumps,
                               const CombinedDefender* constraints, int tricks,
                               bool oppsLead, SUIT oppsSuit) {
  Hand played = Hand::complement(lead, follow, dfndr);
  WorstSearch wd(lead, follow, played, trumps);
  ClaimSP c;
  if (tricks <= 0)  // all
    c = wd.search(constraints);
  else
    c = wd.search(tricks, constraints, oppsLead, oppsSuit);

  if (STATS_ON) {
    // wd.printCacheStats(cerr);
    // wd.bstats.print(cerr);
  }
  return c;
}

void BridgeTestBase::parseClaim(const char* tricks, Claim& c, SUIT trumps,
                                SUIT suitLead) {
  Hand verify;
  bool pdLeads = false;
  istringstream ss(tricks);
  while (ss) {
    string tmp1, tmp2;
    ss >> tmp1;
    ss >> tmp2;
    if (tmp1.size() == 0 || tmp2.size() == 0) continue;

    Play lead(Hand::toSuit(tmp1[0]), Hand::toRank(tmp1[1])),
        follow(Hand::toSuit(tmp2[0]), Hand::toRank(tmp2[1]));
    c.tricks.push_back(ClaimTrick(lead, follow, trumps, pdLeads, suitLead));
    ASSERT_TRUE(!verify.hasCard(lead.suit, lead.rank));
    verify.addCard(lead.suit, lead.rank);
    pdLeads = (pdLeads && c.tricks.rbegin()->leadWins) ||
              (!pdLeads && !c.tricks.rbegin()->leadWins);
  }
}

/* find for two hands, third string is NOT a hand, but the CD string fmt */
ClaimSP BridgeTestBase::runOneConstr(const char* lead, const char* follow,
                                     SUIT trumps, const char* cdef,
                                     int tricks) {
  CombinedDefender cd;

  if (cdef) cd.read(cdef, "");

  return runOne(lead, follow, trumps, cdef ? &cd : NULL,
                tricks);  // no claim unless trumps are 2-1,
}

}  // namespace dtac
