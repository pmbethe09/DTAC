#ifndef _BRIDGE_HANDINCLUDED
#define _BRIDGE_HANDINCLUDED

// Describe a Bridge Hand in bits

// for memset
#include <string.h>

#include <algorithm>
#include <ostream>
#include <sstream>

#include "dtac/Asserts.hpp"
#include "dtac/Constants.hpp"

namespace dtac {

class Hand {
 protected:
  int cards[NUM_SUITS];  // a bit map [SUITS], of RANKS
 public:
  Hand() { std::fill(cards, cards + NUM_SUITS, 0); }
  Hand(const Hand& other) {
    std::copy(other.cards, other.cards + NUM_SUITS, cards);
  }
  Hand& operator=(const Hand& other) {
    std::copy(other.cards, other.cards + NUM_SUITS, cards);
    return *this;
  }

  /*inline*/ RANK getHighestInSuit(SUIT s) const {
    int suitMask = getSuit(s);
    ASSERT_MSG(suitMask, "need some cards in the suit");
    for (RANK r = ACE; r >= TWO; --r) {
      if (suitMask & r) return r;
    }
    throw new bridge_exception("mask had no Ranks in it");
  }

  inline int getSuit(int i) const { return cards[i]; }
  inline bool hasCard(SUIT s, RANK r) const {
    ASSERT_MSG((int)s < 4, "suit idx in range");
    return cards[(int)s] & (int)r;
  }
#ifdef DEBUG
  void removeCard(SUIT s, RANK r);
  void addCard(SUIT s, RANK r);
#else
  inline void removeCard(SUIT s, RANK r) {
    ASSERT_MSG(hasCard(s, r), "card: " << suit2Char(s) << rank2Char(r)
                                       << " not in hand");
    cards[(int)s] ^= (int)r;  // remove this card
  }

  inline void addCard(SUIT s, RANK r) {
    ASSERT_MSG(!hasCard(s, r), "card: " << suit2Char(s) << rank2Char(r)
                                        << " already in hand");
    cards[(int)s] |= (int)r;  // insert this card (could also xor)
  }
#endif
  // helper print method, in a file
  void printHoriz(std::ostream& os) const;
  inline int length(SUIT suit) const {
    // maybe bitmap lookup? for now naive
    int len = 0;
    const int this_suit = cards[suit];
    for (int i = 0; i < NUM_CARDS; ++i)
      if (this_suit & (1 << i)) len++;

    return len;
  }

  bool operator==(const Hand& other) const {
    for (int i = 0; i < NUM_SUITS; ++i) {
      if (cards[i] != other.cards[i]) {
        return false;
      }
    }
    return true;
  }

  inline int size() const {
    return length(SPADES) + length(HEARTS) + length(DIAMONDS) + length(CLUBS);
  }

  //'.' separated PBNStyle
  std::string toString() const {
    static const char* vals = "23456789TJQKA";
    std::ostringstream ss;
    for (int sIdx = 3; sIdx >= 0; --sIdx) {
      // SUIT s = (SUIT)sIdx;
      if (cards[sIdx] > 0) {  // non-empty
        int i = 12;
        for (RANK r = ACE; i >= 0; --r, --i) {
          if ((int)r & cards[sIdx]) ss << vals[i];
        }
      }
      if (sIdx > 0) ss << ".";
    }
    return ss.str();
  }

  // union the two hands
  Hand& operator+=(const Hand& other) {
    for (int i = 0; i < 4; ++i)  // make dfndr1 hold all the defensive cards
      cards[i] |= other.cards[i];
    return *this;
  }

  /////
  /** static methods for enum stuff */
  static inline DIRECTION toDirection(char d) {
    switch (d) {
      case 'N':
      case 'n':
        return NORTH;
      case 'E':
      case 'e':
        return EAST;
      case 'S':
      case 's':
        return SOUTH;
      case 'W':
      case 'w':
        return WEST;
      default:
        return NUM_DIRECTIONS;
    }
  }

  static inline SUIT toSuit(char s) {
    switch (s) {
      case 'C':
      case 'c':
        return CLUBS;
      case 'D':
      case 'd':
        return DIAMONDS;
      case 'H':
      case 'h':
        return HEARTS;
      case 'S':
      case 's':
        return SPADES;
      case 'N':
      case 'n':
        return NOTRUMP;
      default:
        return NUM_SUITS;  // could be NT
    }
  }  // fn

  static inline RANK toRank(char r) {
    switch (r) {
      case 'T':
      case 't':
        return TEN;
      case 'J':
      case 'j':
        return JACK;
      case 'Q':
      case 'q':
        return QUEEN;
      case 'K':
      case 'k':
        return KING;
      case 'A':
      case 'a':
        return ACE;
      default:
        return (RANK)(1 << ((int)r - (int)'2'));
    }
  }  // fn

  static Hand fromString(const std::string& s);

  /* compute 'played' given 2 hands and the defender we want (good for testing)
   */
  static Hand complement(const Hand& one, const Hand& two, const Hand& dfndr) {
    Hand result;
    const int allMask = (((int)ACE) << 1) - 1;
    for (int i = 0; i < (int)NUM_SUITS; ++i) {
      result.cards[i] = one.cards[i] | two.cards[i] | dfndr.cards[i];
      result.cards[i] ^= allMask; /* xor complement */
    }
    return result;
  }
};

Hand operator+(const Hand& lhs, const Hand& other);

}  // namespace dtac

#endif /* inc */
