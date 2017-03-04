#include "dtac/SuitState.h"

using std::vector;

namespace dtac {

// improvement to detect if cards played (see comment)
#define PLAYED_CARDS 1

// detects cards played, to minimize plays
void SuitState::mask2Plays(const Hand& played, int mask, SUIT suit,
                           vector<Play>& plays) {
  bool hadPrev = false;
  const int playedSuit = played.getSuit(suit);
  for (RANK r = TWO; r <= ACE; ++r) {
    if (mask & (int)r) {
      if (!hadPrev) {
        plays.push_back(Play(suit, r));
        hadPrev = true;
      }
// else don't push, keep hadPrev==true
#ifdef PLAYED_CARDS
    } else if (hadPrev && (playedSuit & (int)r)) {
#endif
      // the card below our last has been played...
      // so if we have AQ and the K has been played,
      // the Q will pick up hadPrev.
    } else
      hadPrev = false;
  }
}

// for player in {0,1}
void SuitState::naivePlays(const Hand& played, SUIT suit, int player,
                           vector<Play>& cards) const {
  if (ind_lengths[player][suit] > 0) {
    int mask = current[player]->getSuit(suit);
    mask2Plays(played, mask, suit, cards);
  } else {  // out of suit, try all
    // ordering for discards?
    for (SUIT s = CLUBS; s < NUM_SUITS; ++s)
      mask2Plays(played, current[player]->getSuit(s), s, cards);
  }
}

void SuitState::nextPlaysInSuit(const Hand& played, SUIT suit,
                                bool partnerLeads, vector<Play>& leads,
                                vector<Play>& follows, bool oppsLead) const {
  leads.clear();
  follows.clear();
  int suitIdx = (int)suit;
  // int shortHand = (ind_lengths[0][suitIdx] < ind_lengths[1][suitIdx]) ? 0 :
  // 1;
  // check for high-honors.
  // RANK toPlay;
  // TODO:  as with suits, build a list of cards to try?

  if (ind_lengths[0][suitIdx] == 0 && !oppsLead)
    return;  // nothing to lead in that suit

  // allow 'leader' to 'discard' when oppsLead

  naivePlays(played, suit, 0, leads);
  naivePlays(played, suit, 1, follows);

  /*
    if (hasWinner(shortHand, suitIdx, toPlay)) {
    // try short hand honors, opp low2high from long hand
    } else {
    // try long hand honors, opp low2high from short hand
    }*/

  // for now, naive approach
}

void SuitState::suitOrder(SUIT lastSuit, const WorstSearch& ws,
                          const Claim& claim) {
  int suitIdx = 0;
  // opps out of trump, don't run trumps - no uncond squeezes!
  if (ws.trumps != NOTRUMP &&
      ws.defender.length[(int)ws.trumps] <= claim.round_of[(int)ws.trumps]) {
    ordering[3] = ws.trumps;
  }
  if (lastSuit != NUM_SUITS && lengths[(int)lastSuit] > 0) {
    ordering[suitIdx++] = lastSuit;
    for (int idx = 0; idx < 4; ++idx) {
      SUIT s = (SUIT)idx;
      if (lastSuit == s)
        continue;
      else
        ordering[suitIdx++] = s;
    }
  } else {
    for (int idx = 0; idx < 4; ++idx)
      ordering[idx] = (SUIT)idx;  // init with suit for all
  }

  // naive 'bubble' sort - 3 or 4-elements...
  for (int idx = suitIdx; idx < 3; ++idx) {
    for (int cmp = 0; cmp < (3 - idx); ++cmp) {
      if (lengths[ordering[cmp]] < lengths[ordering[cmp + 1]])  // swap
        std::swap(ordering[cmp], ordering[cmp + 1]);
    }
  }
}

SuitState::SuitState(const WorstSearch& ws, const Claim& claim,
                     bool partnerLeads, SUIT _lastSuit)
    : lastSuit(_lastSuit) {
  current[0] = partnerLeads ? &ws.partner : &ws.origLead;
  current[1] = partnerLeads ? &ws.origLead : &ws.partner;

  for (int i = 0; i < NUM_SUITS; ++i) {
    ind_lengths[0][i] = current[0]->length((SUIT)i);
    ind_lengths[1][i] = current[1]->length((SUIT)i);
    lengths[i] = ind_lengths[0][i] + ind_lengths[1][i];
  }
  suitOrder(lastSuit, ws, claim);
}

}  // namespace dtac
