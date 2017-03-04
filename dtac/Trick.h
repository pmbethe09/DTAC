#ifndef _BRIDGE_TRICK_INC_HPP
#define _BRIDGE_TRICK_INC_HPP

#include <string>
#include <vector>

#include "dtac/Constants.h"
#include "dtac/Hand.h"

namespace dtac {

// internal class
class Trick {
 public:
  Trick(DIRECTION dir, const std::vector<std::string>& data,
        int start_index = 0)
      : offset(dir) {
    const int end_index =
        (start_index + 4 <= data.size() ? start_index + 4 : data.size());
    size = end_index - start_index;
    for (int i = start_index; i < end_index; ++i) {
      // expected format is SuitRank
      const int itr = i - start_index;
      char s = data[i][0];
      if (s != '-') {
        suits[itr] = Hand::toSuit(s);
        char r = data[i][1];
        ranks[itr] = Hand::toRank(r);
      } else {  // '-' is a partial play
        suits[itr] = NUM_SUITS;
        ranks[itr] = TWO;
      }
    }
  }

  RANK ranks[4];  // parallel arrays of rank/suit played (in order)
  SUIT suits[4];
  DIRECTION offset;  // owner of first card (may not be leader)
  int size;

  // given filled in above, determine trick winner based on trump suit
  inline DIRECTION winner(SUIT trump, DIRECTION leader) const {
    int base = ((int)leader - (int)offset) &
               3;  // work from 0 - then rotate mod leader at end
    int wonit = base;
    for (int i = 1; i < size; ++i) {
      int idx = (base + i) & 3;
      if (suits[idx] == suits[wonit]) {
        if ((int)ranks[idx] > (int)ranks[wonit]) wonit = idx;
      } else if (suits[idx] == trump) {
        wonit = idx;
      }
    }

    // now mod for direction using leader.
    wonit = (wonit + (int)offset) & 3;  // mod 4, but faster
    return (DIRECTION)wonit;
  }

  std::string toString() const;
};

}  // namespace dtac

#endif /* HPP */
