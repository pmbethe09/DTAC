#include "dtac/Bid.h"

#include <fstream>
#include <iostream>

#include "dtac/Hand.h"

using std::string;
using std::ostringstream;

namespace dtac {

string Bid::toString() const {
  switch (btype) {
    case DOUBLE:
      return "X";
    case REDOUBLE:
      return "XX";
    case PASS:
      return "P";
    case BID: {
      ostringstream oss;
      oss << level << suit2Char(suit);
      return oss.str();
    }
  }
  ASSERT(false);
}
// level suit
Bid Bid::fromString(const string& s) {
  if (s.size() == 1) {
    if (s[0] == 'X') return Bid(DOUBLE);
    if (s[0] == 'P') return Bid(PASS);
  }
  ASSERT_MSG(s.size() == 2, "not a valid bid:" << s);
  if (s[0] == 'X' && s[1] == 'X') return Bid(REDOUBLE);
  // else a real bid
  int level = ((int)s[0]) - (int)'0';
  ASSERT_MSG(level >= 1 && level <= 7, "level: " << s[0] << " not in [1,7]");
  SUIT suit = Hand::toSuit(s[1]);
  return Bid(level, suit);
}

bool operator<(const Bid& lhs, const Bid& rhs) {
  if (lhs.level < rhs.level) return true;
  if (rhs.level < lhs.level) return false;
  // else levels are ==
  return lhs.suit < rhs.suit;
}

}  // namespace dtac
