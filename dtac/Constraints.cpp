#include "dtac/Constraints.hpp"

#include "dtac/Hand.hpp"

namespace dtac {

static inline void getLen(const Hand &h, int *lengths /* size 4*/,
                          int &totlen) {
  for (int i = 0; i < 4; ++i) {
    lengths[i] = h.length((SUIT)i);
    totlen += lengths[i];
  }
  std::sort(lengths, lengths + 4);
}

static inline bool isBal(int *lengths) {
  return lengths[0] > 1 && lengths[0] + lengths[1] > 4 &&
         lengths[2] + lengths[3] < 9;
}

bool Unbalanced::holds(const Hand &h) const {
  int lengths[4];
  int totlen = 0;
  getLen(h, lengths, totlen);
  if (totlen == 13) return !isBal(lengths);
  // else, during 'creation' avoid bal
  return true;
}

bool Balanced::holds(const Hand &h) const {
  int lengths[4];
  int totlen = 0;
  getLen(h, lengths, totlen);
  if (totlen == 13)
    return isBal(lengths);
  else
    // invariant while making a hand
    return lengths[3] < 6 && lengths[2] + lengths[3] < 9 && lengths[1] < 4;
}

PointRange::PointRange(int low, int high) : low(low), high(high) {}

bool PointRange::holds(const Hand &h) const {
  RANK ranks[] = {JACK, QUEEN, KING, ACE};
  int pts = 0;
  for (int i = 0; i < NUM_SUITS; ++i) {
    SUIT s = (SUIT)i;
    for (int r = 0; r < 4; ++r)
      if (h.hasCard(s, ranks[r])) pts += (r + 1);
  }
  return pts >= low && pts <= high;
}

}  // namespace dtac
