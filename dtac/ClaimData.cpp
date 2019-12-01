#include "dtac/ClaimData.h"

#include <iostream>

using std::cout;
using std::endl;
using std::ostringstream;
using std::string;
using std::vector;

namespace dtac {

void SingleClaim::setHands(const PBNDeal& deal, DIRECTION decl) {
  vector<string> cards(4);
  char firstHand;
  deal.splitDealCards(cards, firstHand);
  DIRECTION firstH = Hand::toDirection(firstHand);
  int leadOffset = ((int)decl - (int)firstH + 4) % 4,
      pdOffset = (leadOffset + 2) % 4, d1 = (leadOffset + 1) % 4,
      d2 = (leadOffset + 3) % 4;

  /* set class members */
  declarer = Hand::fromString(cards[leadOffset]);
  dummy = Hand::fromString(cards[pdOffset]);
  LHO = Hand::fromString(cards[d1]);
  RHO = Hand::fromString(cards[d2]);
}

static void recordHand(const vector<string>& data, Hand& h, int offset) {
  if (data.size() <= offset) return;  // end-of play data
  char s = data[offset][0];
  if (s != '-') {
    SUIT suit = Hand::toSuit(s);
    char r = data[offset][1];
    RANK rank = Hand::toRank(r);
    ASSERT_MSG(h.hasCard(suit, rank), "has the card we are removing");
    h.removeCard(suit, rank);
    return;
  }
  ASSERT_MSG(false, "shouldn't get here");
}

//#define TCONT(call) full = call; if (!full) return false;

void SingleClaim::recordTrick(const vector<string>& data, int start) {
  /* origLeader is also LHO ,etc */
  recordHand(data, LHO, start);
  recordHand(data, dummy, start + 1);
  recordHand(data, RHO, start + 2);
  recordHand(data, declarer, start + 3);
}

/** rip through deal, and determine:
    1. State of hands at the time of claim
    2. Tricks won in record, and thus how many tricks claimed
*/
void SingleClaim::calcClaim(const PBNDeal& deal, bool printOut) {
  reset();

  int roundsIn = deal.plays.size() >> 2;  // div by 4
  bool lastPartial = false;

  int lastRnd = deal.plays.size() & 3;  // mod 4
  if (roundsIn > 0 && lastRnd == 0) lastRnd = 4;

  for (int i = 1; i <= lastRnd; ++i) {
    if (deal.plays[deal.plays.size() - i] == "-") {
      --roundsIn;
      lastPartial = true;
      break;
    }
  }

  char s = deal.contract[1];  // usually '4HX' or the like
  SUIT trump = Hand::toSuit(s);
  DIRECTION decl = Hand::toDirection(deal.declarer[0]);
  int declWins = 0;

  DIRECTION origLeader = Hand::toDirection(deal.play[0]);
  DIRECTION leader = origLeader;

  /* get hands */
  setHands(deal, decl);

  int dealSize = declarer.size();  // usually will be 13
  /* calc tricks won, and record in hands */
  for (int r = 0; r < roundsIn; ++r) {
    const vector<string>& plays = deal.plays;
    Trick tr(origLeader, plays, r * 4);
    // record
    if (!deal.badData) {
      try {
        recordTrick(plays, r * 4);
      } catch (bridge_exception& be) {
        deal.badData = true;
      }
    }

    leader = tr.winner(trump, leader);
    if (((int)leader & 1) ==
        ((int)decl & 1))  // new leader, and decl on same parity
      ++declWins;
    // record trick in hands.
  }
  nextToLead = leader;  //

  int result = atoi(deal.result.c_str());
  if (printOut)
    cout << "Bd:" << deal.board << "Cntr:" << deal.contract
         << " Result:" << deal.result;
  roundsInclaim = dealSize - roundsIn; /* probably 13 - ... , but be sure */
  if (roundsInclaim == 13 && !lastPartial) {
    if (printOut) cout << " no data." << endl;
    deal.badData = true;  // try this
  } else if (roundsInclaim > 0) {
    tricksTakenInclaim = result - declWins;
    ostringstream msg;
    tookAll = false;
    if (roundsInclaim == tricksTakenInclaim) {
      msg << "ALL";
      tookAll = true;
    } else
      // msg << "all but " << (roundsInclaim - tricksTakenInclaim);
      msg << tricksTakenInclaim << "/" << roundsInclaim;
    if (((int)leader & 1) ==
        ((int)decl & 1)) {  // new leader, and decl on same parity
      msg << " and on lead";
      if (tookAll && onLead) {
        onLead->add(tricksTakenInclaim);
      }
    } else if (tookAll && oppsLead) {
      oppsLead->add(tricksTakenInclaim);
    }
    if (printOut)
      cout << " " << declWins << "/" << roundsIn << " -> claim " << msg.str()
           << endl;
  } else if (printOut)
    cout << " played out." << endl;
}  // end fn calcClaim

}  // namespace dtac
