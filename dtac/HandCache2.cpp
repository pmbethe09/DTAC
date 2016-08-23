#include "dtac/HandCache2.hpp"

using std::vector;

namespace dtac {

HandCache2::HandCache2(const Hand& lead, const Hand& follow)
    : stats(new CStats) {
  // init 'map' to cache-key
  // first suits
  card_masks.resize(NUM_SUITS);
  slot_map.resize(NUM_SUITS);

  // int next_mask = 1; // use single set of masks for all cards.
  card_bits = 0;
  for (SUIT s = CLUBS; s <= SPADES; ++s) {
    vector<size_t>& suit_mask = card_masks[(int)s];
    vector<size_t>& slot_mask = slot_map[(int)s];
    for (RANK r = TWO; r <= ACE; ++r) {
      if (lead.hasCard(s, r) || follow.hasCard(s, r)) {
        suit_mask.push_back(r);  // look for this mask
        int next_mask = 1 << card_bits;
        slot_mask.push_back(next_mask);  // give this mask in the mapped version
        ++card_bits;
      }
    }
  }
}

}  // namespace dtac
