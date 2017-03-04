#ifndef _BRIDGE_HANDCACHE1INCLUDED
#define _BRIDGE_HANDCACHE1INCLUDED

/**
 * Take HC0, and add perfect hashing
 * This is the REAL one that we are using
 */

#include <stdint.h>

#include <memory>
#include <unordered_map>
#include <utility>

#include "dtac/Asserts.h"
#include "dtac/CStats.h"
#include "dtac/Claim.h"
#include "dtac/Constants.h"
#include "dtac/Hand.h"
#include "dtac/HandCacheConfig.h"

namespace dtac {

class HandCache1 {
 public:
  HandCache1();
  // for fwd compat
  HandCache1(const Hand& l, const Hand& follow);

  typedef std::pair<uint64_t, uint32_t> hash_key;
  typedef std::shared_ptr<Claim>
      hash_val;  // if a solution, add pointer to the claim obj

  struct PairHash {
    size_t operator()(const hash_key& key) const {
#if defined(__x86_64__)               /* 64-bit mode */
      return key.first ^ key.second;  // try Xor as an initial
#else                                 /* 32-bit mode*/
      size_t low32 = (size_t)key.first, high32 = (size_t)(key.first >> 32);
      return (low32 ^ key.second) ^ high32;  // try Xor as an initial
#endif
    }
  };

  typedef std::unordered_map<hash_key, hash_val, PairHash> cacheType;

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
  bool notInCache(const Hand& origLead, const Hand& partner, int nLosers,
                  const Claim& claim, bool pdLeads) const {
    if (!CACHING_ON) return true;
    cacheType::value_type key =
        getCacheKey(origLead, partner, nLosers, claim, pdLeads);
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

  cacheType::value_type getCacheKey(const Hand& origLead, const Hand& partner,
                                    int nLosers, const Claim& claim,
                                    bool pdLeads) const {
    hash_key::first_type key = 0;
    for (int i = 0; i < (int)NUM_SUITS; ++i) {
      hash_key::first_type suit = origLead.getSuit(i) | partner.getSuit(i);
      if (!suit) continue;
      ASSERT_MSG_DBG(suit < (1 << 13), "suit assert less than 13-bits");
      key |=
          (suit << (13 * i));  // pack first 52-bits with 13-bits from each suit
    }

    // use 52rd bit for who leads (btw - that overwrites
    // the '4s' bit of spades 'round-of' in key
    if (pdLeads) {
      key |= ((uint64_t)1) << 52;
    }
    // else already 0

    // add in played info, can only play a suit 7 rounds to be valid
    // hash_val
    hash_key::second_type val = 0;
    for (int i = 0; i < (int)NUM_SUITS; ++i) {
      ASSERT_MSG_DBG(claim.round_of[i] < 13, "rounds of is sane");
      if (FANCY_KEY) {
        // then take low 3-bits of each number, and add to key (can't take all,
        // run out of memory)
        // take all into a 16-bit 'value' (so most will be zero, make key more
        // efficient)
        if (i != 3) {
          key |= ((hash_key::first_type)(claim.round_of[i] & 7))
                 << (53 + 3 * i);
        } else {  // only 2-bits for i== 3
          key |= ((hash_key::first_type)(claim.round_of[i] & 4))
                 << (53 + 3 * i);
        }
      }
      // regardless take 4-bit number (0-15), and pack in 16-bits
      val |= (claim.round_of[i] << (i * 4));
    }
    // add n Losers, prob in range 0-2
    val |= (nLosers << 16);

    return std::make_pair(std::make_pair(key, val), std::shared_ptr<Claim>());
  }

  // to share cache
  HandCache1& operator=(const HandCache1& other) {
    visited = other.visited;
    return *this;
  }

  HandCache1(const HandCache1& other) : visited(other.visited) {}

 protected:
  void add(cacheType::value_type key) {
    if (!CACHING_ON) {
      return;
    }

    if (STATS_ON) {
      if (visited.find(key.first) != visited.end()) {
        ++stats->collisions;
      }
    }
    visited.insert(key);  // kick out a collision
  }

  // hash-key is 52-bits of 4 suits, 1 bit of who is on lead (2 if 4-player
  // hash)
  // low 2-bits of rounds-of 4 suits => 61 bit key.  Then val is all 4 suits
  // 4-bits
  // rounds of
  cacheType visited;
};  // end class

}  // namespace dtac

#endif /* hdr */
