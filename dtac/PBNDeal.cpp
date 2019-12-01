#include "dtac/PBNDeal.h"

#include <sstream>

using std::istringstream;
using std::ostream;
using std::pair;
using std::string;
using std::vector;

// this is in PBNFile.cpp, to get access to the enums
// void PBNDeal::printDeal(ostream & os) const;

void PBNDeal::splitDealCards(vector<string>& cards, char& firstHand) const {
  istringstream ss(deal);
  char dummy;

  ss >> firstHand;  // first char is dir
  ss >> dummy;      // burn the ':'

  cards.resize(4);
  for (int i = 0; i < 4; ++i) {
    ss >> cards[i];  // 4 hands - space separated
  }
}
