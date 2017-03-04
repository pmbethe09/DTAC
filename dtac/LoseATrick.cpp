#include "dtac/LoseATrick.h"

#include "dtac/Claim.h"

using std::shared_ptr;
using std::vector;

namespace dtac {

/// Lose A Trick
void LoseATrick::setDuckLine(const Claim& claim) {
  theyDuck = shared_ptr<Claim>(new Claim(claim));  // copy
  // but remove stuff before this round
  vector<ClaimTrick>& tricks = theyDuck->tricks;
  // lost trick included in parent claim
  // so just tricks after duck
  tricks.erase(tricks.begin(), tricks.begin() + (round));
}

}  //  namespace dtac
