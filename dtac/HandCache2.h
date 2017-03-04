#ifndef _BRIDGE_HANDCACHE2INCLUDED
#define _BRIDGE_HANDCACHE2INCLUDED /* intentional */

/**
 * provide mechanism to cache visited nodes
 * using bit representation of the position
 */

#include <stdint.h>

#include <unordered_map>
#include <vector>

#include "dtac/CStats.h"
#include "dtac/Claim.h"
#include "dtac/Hand.h"
#include "dtac/HandCacheConfig.h"

namespace dtac {

/* specific to start position of two hands! (and length of suits?)
* also deal with 32/64-bit*/

class HandCache2 {
 public:
  HandCache2(const Hand& lead, const Hand& follow);

  typedef uint64_t hash_key;
  typedef std::shared_ptr<Claim>
      hash_val;         // if a solution, add pointer to the claim obj
#if defined(__x86_64__) /* 64-bit mode */
  struct PairHash {
    size_t operator()(const hash_key& key) const { return key; }
  };
  typedef std::unordered_map<hash_key, hash_val, PairHash> cacheType;
#else /* 32-bit mode */
  struct PairHash {
    size_t operator()(const hash_key& key) const {
      size_t low32 = (size_t)key, high32 = (size_t)(key >> 32);
      return low32 ^ high32;  // try Xor as an initial
    }
  };
  typedef std::unordered_map<hash_key, hash_val, PairHash> cacheType;
#endif

  // add current state of reminaing cards + suits to cache
  // 52 bits of S|H|D|C remaining holding of players (combined)
  // 1 bit is the leader
  // 8 low-bits of rounds-of
  // 'value' is the rest of 'rounds-of'
  void addToCache(cacheType::value_type key) { add(key); }

  // found a solution from this point.  Save that info
  void addSolution(cacheType::value_type key, std::shared_ptr<Claim> claim) {
    cacheType::iterator it(visited.find(key.first));
    ASSERT_MSG(it != visited.end(),
               "must be already in cache to append a soln");
    it->second = claim;
  }

  std::shared_ptr<Claim> getSolution(cacheType::value_type key) const {
    cacheType::const_iterator it(visited.find(key.first));
    bool nope = it == visited.end();
    return nope ? std::shared_ptr<Claim>() : key.second;
  }

  // find out if a state is in the cache
  bool notInCache(const Hand& origLead, const Hand& partner, const Claim& claim,
                  bool pdLeads) const {
    if (!CACHING_ON) return true;
    cacheType::value_type key = getCacheKey(origLead, partner, claim, pdLeads);
    return notInCache(key);
  }
  bool notInCache(cacheType::value_type key) const {
    if (!CACHING_ON) return true;
    cacheType::const_iterator it(visited.find(key.first));
    bool nope = it == visited.end();
    if (STATS_ON) {
      if (nope)
        ++stats->misses;
      else
        ++stats->hits;
    }
    return nope;
  }
  void reset() { visited.clear(); }

  std::shared_ptr<CStats> stats;  // if taken

 public:
  cacheType::value_type getCacheKey(const Hand& origLead, const Hand& partner,
                                    const Claim& claim, bool pdLeads) const {
    hash_key key = 0;
    for (int i = 0; i < (int)NUM_SUITS; ++i) {
      const std::vector<size_t>& suit_mask = card_masks[i];
      const std::vector<size_t>& slot_mask = slot_map[i];
      hash_key suit = origLead.getSuit(i) | partner.getSuit(i);
      if (!suit) continue;
      ASSERT_MSG_DBG(suit < (1 << 13), "assert less than 13-bits");
      for (int b = 0, size = suit_mask.size(); b < size; ++b) {
        if (suit & suit_mask[b]) key |= slot_mask[b];
      }
    }

    // use first bit after card-portion for who leads (btw - that overwrites
    // the '4s' bit of spades 'round-of' in key
    if (pdLeads) key |= (1L << card_bits);
    // else already 0

    // add in played info, can only play a suit 7 rounds to be valid
    // hash_val
    hash_key val = 0;
    for (int i = 0; i < (int)NUM_SUITS; ++i) {
      ASSERT_MSG_DBG(claim.round_of[i] < 16, "sane roundsof");
      // take 4-bit number (0-15), and pack in 16-bits
      val |= (claim.round_of[i] << (i << 2));
    }
    key |= (val << (card_bits + 1));  // append bit string

    //    cerr << std::hex << key << endl;

    return std::make_pair(key, std::shared_ptr<Claim>());
  }

  // to share cache
  HandCache2& operator=(const HandCache2& other) {
    visited = other.visited;
    return *this;
  }

  HandCache2(const HandCache2& other) : visited(other.visited) {}

 protected:
  void add(cacheType::value_type key) {
    if (!CACHING_ON) return;

    if (STATS_ON) {
      if (visited.find(key.first) != visited.end()) ++stats->collisions;
    }
    visited.insert(key);  // kick out a collision
  }

 protected:
  // hash-key is n-bits of 4 suits (up to 26), 1 bit of who is on lead (2 if
  // 4-player hash)
  // low 2-bits of rounds-of 4 suits => 61 bit key.  Then val is all 4 suits
  // 4-bits
  // rounds of
  cacheType visited;

  // 2nd-phase cache fields

  // each vector's outer-dimension is [SUIT]
  std::vector<std::vector<size_t> > card_masks;  // set of masks to look for,
                                                 // map if (card_masks[suit][i]
                                                 // & hand[suit])
  std::vector<std::vector<size_t> >
      slot_map;   // then add slot_map[suit][i] to mask
  int card_bits;  // count of bits used by card-portion
};                // end class

}  // namespace dtac

#endif /* hdr */
