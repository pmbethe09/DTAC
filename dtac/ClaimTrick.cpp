#include "dtac/ClaimTrick.hpp"

#include "dtac/Claim.hpp"
#include "dtac/CombinedDefender.hpp"

using namespace std;

namespace dtac {

static bool leaderWins(const Play& lead, const Play& follow, SUIT trumps,
                       SUIT suitLead) {
  if (follow.suit == lead.suit) return follow.rank < lead.rank;
  if (lead.suit == trumps) return true;
  if (follow.suit == suitLead)  // (not same as 'lead', but same as oppsLead
    return false;
  return trumps == NOTRUMP || follow.suit != trumps;
}

ClaimTrick::ClaimTrick(const ClaimTrick& other)
    : lead(other.lead),
      follow(other.follow),
      pdLeads(other.pdLeads),
      leadWins(other.leadWins),
      suitLead(other.suitLead),
      winner(other.leadWins ? &lead : &follow) {}

ClaimTrick& ClaimTrick::operator=(const ClaimTrick& other) {
  lead = other.lead;
  follow = other.follow;
  leadWins = other.leadWins;
  pdLeads = other.pdLeads;
  suitLead = other.suitLead;
  winner = (leadWins ? &lead : &follow);
  return *this;
}

ClaimTrick::ClaimTrick(const Play& _lead, const Play& _follow, SUIT trumps,
                       bool _pdLeads, SUIT _sLead)
    : lead(_lead), follow(_follow), pdLeads(_pdLeads), suitLead(_sLead) {
  leadWins = leaderWins(lead, follow, trumps, suitLead);
  winner = (leadWins ? &lead : &follow);
  if (suitLead == NOTRUMP) suitLead = lead.suit;
}

/* can the CD beat us on this trick */
bool ClaimTrick::beats(const Claim& claim, const CombinedDefender& dfndr,
                       SUIT trumps) const {
  if (suitLead == lead.suit || winner->suit == suitLead) {
    // we are on lead,
    // or following suit to their lead
    int wsIdx = (int)winner->suit;
    bool trumpsDrawn = trumps == NOTRUMP ||
                       dfndr.length[(int)trumps] <=
                           claim.round_of[(int)trumps];  // drawn, or never had
    /** if we are in a trump contract,
     * the dfndr has trumps when the winner is not
     * and they don't have to follow to the lead */
    if (winner->suit != trumps && trumps != NOTRUMP && !trumpsDrawn &&
        claim.round_of[wsIdx] >= dfndr.min_length[wsIdx])
      // and defender can ruff
      return false;
    // else either NOTRUMP, or winning card is in trump -- so simple rank test

    if (!leadWins && lead.suit != follow.suit &&
        winner->suit == trumps && /* we are trumping */
        /* and opps are out of trump, or must follow to suit lead */
        (trumpsDrawn ||
         claim.round_of[(int)lead.suit] < dfndr.min_length[(int)lead.suit]))
      return true;

    int sIdx = (int)winner->suit;
    if (claim.round_of[sIdx] >= dfndr.length[sIdx])
      return true; /* defenders are out of the winning suit */

    if (winner->rank > dfndr.highest[sIdx]) return true;
    return false;
  } else {
    // opps are on lead and we are out - can only ruff
    if (trumps == NOTRUMP || winner->suit != trumps) return false;
    if (dfndr.length[trumps] == 0 || dfndr.highest[trumps] < winner->rank)
      return true;
    return false;
  }
}

string ClaimTrick::toString() const {
  string result = lead.toString() + " " + follow.toString();
  return result;
}

}  // namespace dtac
