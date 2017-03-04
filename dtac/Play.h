#ifndef _BRIDGE_PLAY_INCLUDED_HPP
#define _BRIDGE_PLAY_INCLUDED_HPP

#include "dtac/Constants.h"
#include "dtac/Hand.h"

namespace dtac {

class Play {
 public:
  SUIT suit;
  RANK rank;
  Play() : suit(NUM_SUITS), rank(TWO) {}
  Play(SUIT _suit, RANK _rank) : suit(_suit), rank(_rank) {}
  bool beats(const Play& prev, SUIT trump) const {
    if (prev.suit == suit) {
      return prev.rank < rank;
    }
    if (suit == trump) {  // this is trump, and not prev, then
      return true;
    }
    return false;
    // Assumes that 'NUM_SUITS' will never be in this->suit
  }
  void play(Hand& hand) const { hand.removeCard(suit, rank); }
  void unplay(Hand& hand) const { hand.addCard(suit, rank); }

  inline RANK winCheaply(const Hand& hand) const {
    RANK r = rank;
    ++r;
    for (; r <= ACE; ++r) {
      if (hand.hasCard(suit, r)) {
        break;
      }
    }
    return r;
  }

  bool operator==(const Play& right) const {
    return suit == right.suit && rank == right.rank;
  }
  std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Play& p);

}  // namespace dtac

#endif  //.h
