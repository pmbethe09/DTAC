#include "dtac/Hand.hpp"

using std::endl;
using std::ostream;

namespace dtac {

Hand operator+(const Hand& lhs, const Hand& other) {
  return Hand(lhs) += other;
}

void Hand::printHoriz(ostream& os) const { os << toString() << endl; }

static Hand linParse(const std::string& s) {
  throw bridge_exception("linparse not implemened: " + s);
}

Hand Hand::fromString(const std::string& s) {
  if (s.size() > 0 && s[0] == 'S') {
    return linParse(s);
  }
  Hand result;
  SUIT curr = SPADES;
  int idx = 0;
  RANK imposs = (RANK)((int)ACE + 1),
       last = imposs;  // 'impossible' card, greater than ace
  while (idx < s.size()) {
    char c = s[idx];
    if (c == '-' || c == ',' || c == '.') {
      --curr;
      last = imposs;  // reset
    } else {
      RANK r = toRank(c);
      if (r >= last) {  // new suit
        --curr;
      }
      if (curr < CLUBS) {
        throw bridge_exception("problem parsing the hand: " + s);
      }
      result.cards[(int)curr] |= (int)r;
      last = r;
    }
    ++idx;
  }  // end while
  return result;
}

#ifdef DEBUG
// then not inline, for debugging
void Hand::removeCard(SUIT s, RANK r) {
  ASSERT_MSG_DBG(hasCard(s, r), "card: " << suit2Char(s) << rank2Char(r)
                                         << " not in hand");
  static int ALL_CARDS = (((int)ACE) << 1) - 1;  // all bits from 2 to A
  cards[(int)s] ^= (int)r;                       // remove this card
  // cards[(int)s] &= (((int)r) ^ ALL_CARDS); // safe way, makes no change even
  // if card missing
}
void Hand::addCard(SUIT s, RANK r) {
  // ASSERT_MSG_DBG(!hasCard(s, r), "card: " << suit2Char(s) << rank2Char(r) <<
  // " already in hand");
  if (hasCard(s, r)) {
    ASSERT_MSG_DBG(false, "card: " << suit2Char(s) << rank2Char(r)
                                   << " already in hand");
  }
  cards[(int)s] |= (int)r;  // insert this card (could also xor)
}
#endif

}  // namespace dtac
