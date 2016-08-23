#ifndef _BRIDGE_CLAIM_TRICK_INCLUDED_HPP
#define _BRIDGE_CLAIM_TRICK_INCLUDED_HPP

#include "dtac/Constants.hpp"
#include "dtac/Play.hpp"

namespace dtac {

class Claim;
class CombinedDefender;

/** for claims only lead or follow can win */
class ClaimTrick {
 public:
  Play lead, follow;
  bool pdLeads, leadWins;

  SUIT suitLead;  // likely lead.suit, but could be different if oppsLead
  Play* winner;
  ClaimTrick(const Play& lead, const Play& follow, SUIT trumps, bool pdLeads,
             SUIT oppsLead = NOTRUMP);
  ClaimTrick()
      : pdLeads(false), leadWins(false), suitLead(NOTRUMP), winner(NULL){};
  ClaimTrick(const ClaimTrick& other);
  ClaimTrick& operator=(const ClaimTrick& other);
  bool beats(const Claim& claim, const CombinedDefender& dfndr,
             SUIT trumps) const;
  bool operator==(const ClaimTrick& right) const {
    return lead == right.lead && follow == right.follow;
  }
  std::string toString() const;
};

}  // namespace dtac

#endif
