#ifndef _BRIDGE_CLAIMINCLUDED
#define _BRIDGE_CLAIMINCLUDED

#include <algorithm>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "dtac/ClaimTrick.hpp"
#include "dtac/CombinedDefender.hpp"
#include "dtac/Hand.hpp"
#include "dtac/LoseATrick.hpp"

namespace dtac {

class DotHelper;  // pre-declare

// ordering of the cards
class Claim {
  friend class DotHelper;

 public:
  // ranks[i] and suits[i] is the pair representing that play
  std::vector<ClaimTrick> tricks;

  //  vector<LoseATrick> lostTricks;
  std::shared_ptr<LoseATrick> lostTrick;

  int toTake;
  int round_of[NUM_SUITS];  // keep track of the rounds of this suit led
  explicit Claim(int _toTake) : toTake(_toTake) {
    std::fill(round_of, round_of + NUM_SUITS, 0);
  }
  Claim(const Claim& other);

  void reset(int toTk = -1) {
    tricks.resize(0);
    std::fill(round_of, round_of + NUM_SUITS, 0);
    if (toTk > 0) toTake = toTk;
  }

  void print(std::ostream& os, const std::string& indent = "") const;
  void printDot(std::ostream& os) const;
  void printDot(const std::string& fname) const;
  bool verify(const CombinedDefender& cd, SUIT trumps) const;

 protected:
  void printDot(std::ostream& os, int parent, int& label,
                const std::string& lbstr) const;
};

}  // namespace dtac

#endif /* hdr */
