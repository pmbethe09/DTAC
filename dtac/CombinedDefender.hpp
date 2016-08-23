#ifndef _BRIDGE_COMBINEDDEFENDER_INC
#define _BRIDGE_COMBINEDDEFENDER_INC

/*
 *   Copyright (C) 2009 by Paul Bethe   *
 *   pmb309@cs.nyu.edu   *
 *
 * combined defender for the Worst Case Search
 * class
 ***************************************************************************/

// for memset
#include <string.h>

#include <istream>
#include <ostream>
#include <string>

#include "dtac/Hand.hpp"

namespace dtac {

class CombinedDefender {
 public:
  RANK highest[NUM_SUITS];    // opps best card in each suit
  int length[NUM_SUITS];      // max num of rounds they can follow
  int min_length[NUM_SUITS];  // max num of rounds they can follow
  Hand origHolding;

  CombinedDefender() {
    memset(highest, 0, sizeof(highest));
    memset(length, 0, sizeof(length));
    memset(min_length, 0, sizeof(min_length));
  }
  void init(const Hand& origLead, const Hand& partner, const Hand& played);

  void override(const CombinedDefender& other);
  std::ostream& write(std::ostream& os) const;
  std::istream& read(std::istream& os);
  void read(const std::string& s, const std::string& allCards);
  void read(const std::string& s, const Hand& origH);

  CombinedDefender(const CombinedDefender& other) {
    memcpy(length, other.length, sizeof(length));
    memcpy(min_length, other.min_length, sizeof(min_length));
    memcpy(highest, other.highest, sizeof(highest));
    origHolding = other.origHolding;
  }

 private:
  CombinedDefender& operator=(const CombinedDefender& other);
};

}  // namespace dtac

#endif /* inc */
