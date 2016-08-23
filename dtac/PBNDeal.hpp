#ifndef _PBN_DEAL_HPP
#define _PBN_DEAL_HPP

/** represents a single Deal from PBN */

#include <sstream>
#include <string>
#include <vector>

class PBNDeal {
 public:
  PBNDeal() : badData(false) {}
  std::string event;
  std::string site;
  std::string date;
  std::string round;
  std::string section;
  std::string board;
  std::string west;
  std::string north;
  std::string east;
  std::string south;
  std::string dealer;
  std::string vulnerable;
  std::string deal;
  std::string scoring;
  std::string declarer;
  std::string contract;
  std::string result;
  std::string hometeam;
  std::string room;
  std::string score;
  std::string stage;
  std::string visitteam;
  std::string auction;
  std::string play;

  /* preserve above order and starting at beginning for memory hack */
  std::vector<std::string> bids;
  // each explanation, with an index that points to the bid in 'bids'
  std::vector<std::pair<int, std::string> > bid_explanation;
  std::vector<std::string> plays;
  // same as for bids
  std::vector<std::pair<int, std::string> > play_explanation;

  std::vector<std::pair<std::string, std::string> > optional_tags;
  std::vector<std::string> commentary;

  // marker
  mutable bool badData;
  // standard markup output (in PBNFile.cpp)
  void printDeal(std::ostream& os) const;
  // to split the deal string into a vector of 4 strings
  void splitDealCards(std::vector<std::string>& cards, char& firstHand) const;
};

#endif /* HPP */
