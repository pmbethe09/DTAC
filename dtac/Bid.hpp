#ifndef _BRIDGE_Bid_H_INCLUDED
#define _BRIDGE_Bid_H_INCLUDED

#include <string>

#include "dtac/Asserts.hpp"
#include "dtac/Constants.hpp"

namespace dtac {

// really a 'call', as we include P, X, XX
class Bid {
 public:
  enum BidType { BID, DOUBLE, REDOUBLE, PASS } btype;
  int level;  // 1-7
  SUIT suit;  // any of the 5 suits. (inc NT)

  explicit Bid(BidType bt = PASS) : btype(bt), level(0), suit(NUM_SUITS) {
    ASSERT_MSG(bt != BID, "use level/suit constructor for real bids");
  }
  Bid(int level, SUIT suit) : btype(BID), level(level), suit(suit) {}

  std::string toString() const;
  static Bid fromString(const std::string& s);
};

// for easy comparison
bool operator<(const Bid& lhs, const Bid& rhs);

}  // namespace dtac

#endif  // hpp
