#ifndef _BRIDGE_LOSE_A_TRICK_INCLUDED_HPP
#define _BRIDGE_LOSE_A_TRICK_INCLUDED_HPP

#include <memory>
#include <vector>

#include "dtac/Constants.hpp"

namespace dtac {

class Claim;

/* If Claim-K, and we drive out an Ace or opp ducks
 * basically
 */
class LoseATrick {
 public:
  std::vector<std::shared_ptr<Claim> >
      ruffSuitClaims;  // entry for each suit they can lead
  std::vector<std::shared_ptr<Claim> >
      winSuitClaims;  // entry for each suit they can lead
  std::shared_ptr<Claim> theyDuck;
  int round;  // which round

  LoseATrick()
      : ruffSuitClaims(NUM_SUITS), winSuitClaims(NUM_SUITS), round(-1) {}
  explicit LoseATrick(int _round)
      : ruffSuitClaims(NUM_SUITS), winSuitClaims(NUM_SUITS), round(_round) {}

  void reset() {
    round = -1;
    theyDuck = std::shared_ptr<Claim>();
    for (int i = 0; i < NUM_SUITS; ++i) {
      ruffSuitClaims[i] = std::shared_ptr<Claim>();
      winSuitClaims[i] = std::shared_ptr<Claim>();
    }
  }

  LoseATrick(const LoseATrick& other)
      : ruffSuitClaims(other.ruffSuitClaims),
        winSuitClaims(other.winSuitClaims),
        theyDuck(other.theyDuck),
        round(other.round) {}

  LoseATrick& operator=(const LoseATrick& other) {
    ruffSuitClaims = other.ruffSuitClaims;
    winSuitClaims = other.winSuitClaims;
    theyDuck = other.theyDuck;
    round = other.round;
    return *this;
  }

  // given claim, (and preset round), save after duck part of claim
  void setDuckLine(const Claim& claim);
  //  bool lostOne() const { return round != -1; }
};
}  // namespace dtac

#endif  // hpp
