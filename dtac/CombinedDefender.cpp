/***************************************************************************
 *   Copyright (C) 2009 by Paul Bethe   *
 *   pmb309@cs.nyu.edu   *
 *
 * Worst Case Search
 * the opponents play the best card they have on every possilbe round of
 * play in that suit
 * eg:  we have KQxx opposite ATx -
 * SO: opponents hold J 6th... play the Jack for up to 6 rounds. *
 * Will discover to play A K Q in separate plays - only 3 winners.
 ***************************************************************************/
#include "dtac/CombinedDefender.h"

using std::endl;
using std::istringstream;
using std::ostream;
using std::istream;
using std::string;

namespace dtac {

// sift-thru known info to create joint-"defender"
// * max-length outstanding
// * highest card outstanding
void CombinedDefender::init(const Hand& origLead, const Hand& partner,
                            const Hand& played) {
  int tricks_remaining = 0;
  origHolding = Hand::complement(origLead, partner, played);
  for (int i = 0; i < (int)NUM_SUITS; ++i) {
    length[i] = origHolding.length((SUIT)i);
    tricks_remaining += length[i];
    if (origHolding.getSuit(i))  // has cards in the suit
      highest[i] = origHolding.getHighestInSuit((SUIT)i);
  }
  tricks_remaining /= 2;
  if (tricks_remaining == 0) return;
  for (int i = 0; i < (int)NUM_SUITS; ++i)
    min_length[i] = length[i] / tricks_remaining;
}

/** apply tighter restrictions from other onto this
 * - so min_length > than ours,
 * - or length < ours (but non-zero)
 */
void CombinedDefender::override(const CombinedDefender& other) {
  for (int i = 0; i < NUM_SUITS; ++i) {
    if (other.length[i] >= 0 && other.length[i] < length[i])
      length[i] = other.length[i];  // restrict to a more favorable distro
    if (other.min_length[i] > min_length[i])
      min_length[i] = other.min_length[i];  // more even break
    if (other.highest[i] < highest[i])      // high card was played
      highest[i] = other.highest[i];
  }
}

// converse of print out
void CombinedDefender::read(const string& s, const string& allCards) {
  read(s, Hand::fromString(allCards));
}

void CombinedDefender::read(const string& s, const Hand& origH) {
  istringstream is(s);
  read(is);
  origHolding = origH;
}

istream& CombinedDefender::read(istream& is) {
  // x: (r, M, m);
  // assumes a very specific format...
  // FIXME !what about second holdings in a suit
  for (int sIdx = 3; sIdx >= 0; --sIdx) {
    string s, r, M, m;
    is >> s >> r >> M >> m;
    ASSERT_MSG(s[0] == suit2Char((SUIT)sIdx), "assert suit matches expected");
    highest[sIdx] = Hand::toRank(r[1]);
    length[sIdx] = (int)M[0] - (int)'0';
    min_length[sIdx] = (int)m[0] - (int)'0';
  }
  return is;
}

// pretty print
ostream& CombinedDefender::write(ostream& os) const {
  for (int sIdx = 3; sIdx >= 0; --sIdx) {
    os << suit2Char((SUIT)sIdx) << ": (" << rank2Char(highest[sIdx]) << ", "
       << length[sIdx] << ", " << min_length[sIdx] << "); ";
  }
  os << endl;
  return os;  // for cascading
}

}  // namespace dtac {
