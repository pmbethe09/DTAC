#ifndef SUITSTATE_INC
#define SUITSTATE_INC

/**
 * Suit State keeps all the needed info for a particular depth of search
 *
 * also provides some helper functions, like looking up suit ordering, or play
 * order
 */

#include <vector>

#include "dtac/Claim.hpp"
#include "dtac/Constants.hpp"
#include "dtac/Hand.hpp"
#include "dtac/WorstSearch.hpp"

namespace dtac {

class SuitState {
 public:
  explicit SuitState(const WorstSearch& ws, const Claim& claim,
                     bool partnerLeads, SUIT _lastSuit);

  //
  SUIT lastSuit;  // save input
  int ind_lengths[2][NUM_SUITS];
  int lengths[NUM_SUITS];
  const Hand* current[2];
  SUIT ordering[NUM_SUITS];

  /* determine if this hand has a winner
     bool hasWinner(int shortHand, int suitIdx, RANK & toPlay) const {
     // TODO : figure out
     return false;
     }*/

  /* determine list of valid plays */
  void nextPlaysInSuit(const Hand& played, SUIT suit, bool partnerLeads,
                       std::vector<Play>& leads, std::vector<Play>& follows,
                       bool oppsLead) const;

  static void mask2Plays(const Hand& played, int mask, SUIT suit,
                         std::vector<Play>& plays);

 private:
  // fill in the 'ordering' field
  //  calc best guesses of suits to lead, with caveat to continue current suit
  //  if length > 0
  void suitOrder(SUIT lastSuit, const WorstSearch& ws, const Claim& claim);
  // for player in {0,1}
  void naivePlays(const Hand& played, SUIT suit, int player,
                  std::vector<Play>& cards) const;
};

}  // namespace dtac

#endif /* HPP */
