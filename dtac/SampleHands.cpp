#include "dtac/SampleHands.h"

#include <stdlib.h>

#include <iomanip>
#include <iostream>

#include "dtac/ClaimData.h"  // for stddev
#include "dtac/Constraints.h"
#include "dtac/Hand.h"

using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::setprecision;
using std::vector;

namespace dtac {

static long int nrands = 0;

Hand SampleHandsBase::getRandom(const Hand& pdHand, int numCards) {
  cout << "getRandom(" << pdHand.toString() << ")" << endl;
  Hand res;
  Hand deck = Hand::complement(pdHand, Hand(), Hand());
  int size = 0;

  while (size < numCards) {
    int rand = random() % 52;
    ++nrands;
    SUIT s = (SUIT)(rand / 13);
    RANK r = TWO + (rand % 13);
    while (!deck.hasCard(s, r)) {
      if (r != ACE)
        ++r;
      else {
        r = TWO;
        if (s != SPADES)
          ++s;
        else
          s = CLUBS;
      }
    }
    res.addCard(s, r);
    deck.removeCard(s, r);
    ++size;
  }
  return res;
}

void SampleHandsBase::setSeed(long int seed) { srandom(seed); }

Hand SampleHandsBase::getConstrainedRandom(
    const Hand& pdHand, const vector<const IConstraint*>& constraints,
    int numCards) {
  Hand result;
  while (1) {
    result = getRandom(pdHand, numCards);
    size_t i = 0;  // to see if loop completed
    for (; i < constraints.size(); ++i)
      if (!constraints[i]->holds(result)) break;
    if (i == constraints.size()) return result;
    // TODO - break out?
  }
}

pair<double, Hand> SampleHandsRandom::getConstrained(const Hand& pdHand,
                                                     const PointRange& pr,
                                                     const IConstraint& shape,
                                                     int numCards) const {
  vector<const IConstraint*> constraints(2);
  constraints[0] = &pr;
  constraints[1] = &shape;
  return make_pair(1., getConstrainedRandom(pdHand, constraints, numCards));
}

SampleReport SampleHandsRandom::sampleNT(long int seed,
                                         const Hand& pdHand) const {
  SampleHandsBase::setSeed(seed);

  nrands = 0;  // global
  PointRange pr(15, 17);
  Balanced bal;
  // sample random - then ensure
  int tot = 0, nt = 0;
  while (nt < 100) {
    Hand h = getRandom(pdHand, 13 /* numCards */);
    if (bal.holds(h) && pr.holds(h)) ++nt;
    ++tot;
  }

  return SampleReport(tot, nt, nrands);
}

#if 0
// custom for 0-4 range
size_t h2idx(size_t numa, size_t numk, size_t numq, size_t numj) {
  return numj | (numq << 3) |  (numk << 6) |  (numa << 9) ;
}

void createHonorMats() {
  vector<size_t> pts(h2idx(4,4,4,1) + 1);
  cout << pts.size() << endl;
  // ordered vector of cumulative prob, 'idx'
  // where idx maps to a specfic number of each honor type
  vector<pair<double, size_t> > poss1517;
  vector<double> honProbs(14);

  vector<double> factHon(14, 1.), factSpot(14, 1.);
  for (int i=1; i <= 13; ++i) {
    // if 3 honors, then adjust /(52*51*50) to /(42*41*40), etc
    factHon[i] = (17-i)*factHon[i-1]/(39+i);
    factSpot[i] = (37-i)*factSpot[i-1]/(53-i);
  }

  // combine together - still prob of 'x' honors
  for (int i=0; i <= 13; ++i) {
    honProbs[i] = factHon[i] * factSpot[13-i];
  }

  double cumProb=0.;
  for (size_t numa=0; numa < 4; ++numa) {
    for (size_t numk=0; numk < 4; ++numk) {
      for (size_t numq=0; numq < 4; ++numq) {
        for (size_t numj=0; numj < 4; ++numj) {
          if (numa+numk+numq+numj> 13) continue;
          int idx = h2idx(numa, numk, numq, numj);
          pts[idx] = 4*numa + 3* numk + 2*numq + numj;
          int nhons = numa + numk + numq + numj;
          // TODO custom ranges
          if (pts[idx] >= 15 && pts[idx] <= 17) {
            cumProb += honProbs[nhons];
            poss1517.push_back(make_pair(cumProb, idx));
          }
        }
      }
    }
  }

  // now done, normalize entries with total
  for (size_t i=0; i < poss1517.size(); ++i)
    poss1517[i].first /= cumProb;
}
#endif

// Using rank2Int, where JACK => 9
static int pointTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4};

// naive scan
static bool getIdxCard(const Hand& hand, int idx, SUIT* suit, RANK* rank) {
  int cnt = 0;
  for (SUIT s = CLUBS; s < NUM_SUITS; ++s) {
    for (RANK r = TWO; r <= ACE; ++r) {
      if (hand.hasCard(s, r)) {
        if (cnt == idx) {
          *suit = s;
          *rank = r;
          return true;
        }
        ++cnt;
      }
    }
  }
  return false;
}

static int complementHonors(const Hand& held, Hand* result) {
  int points = 0;
  for (SUIT s = CLUBS; s < NUM_SUITS; ++s) {
    for (RANK r = JACK; r <= ACE; ++r) {
      if (!held.hasCard(s, r)) {
        result->addCard(s, r);
        points += pointTable[rank2Int(r)];
      }
    }
  }
  return points;
}

pair<double, Hand> SampleHandsErnie::getConstrained(const Hand& pdHand,
                                                    const PointRange& pr,
                                                    const IConstraint& shape,
                                                    int numCards) const {
  Hand result;
  double prob = 1.0;
  Hand justHonorsHand;
  int points = complementHonors(pdHand, &justHonorsHand);
  Hand lowDeck = Hand::complement(pdHand, justHonorsHand, Hand());

  while (result.size() < numCards) {
    const size_t hsize = justHonorsHand.size();
    if (hsize == 0) {
      if (points >= pr.low && points <= pr.high)
        break;  // we are fine
      else
        throw bridge_exception(
            "fail, no honors available, requested > 0 points");
    }
    if (points <= pr.high) {  // 1/2 chance to exit/continue
      prob /= 2;
      ++nrands;
      if ((random() & 1)) {
        break;
      }
    }
    int rand = random() % hsize;
    ++nrands;
    prob /= hsize;
    // take this card, and remove from pack

    SUIT s;
    RANK r;
    if (!getIdxCard(justHonorsHand, rand, &s, &r)) {
      throw bridge_exception("card for idx not found");
    }
    if (points - pointTable[rank2Int(r)] < pr.low) {
      continue;  // try again -- note inf loop possible
    }
    result.addCard(s, r);
    justHonorsHand.removeCard(s, r);
    points -= pointTable[rank2Int(r)];
  }  // while

  // now points criteria is met, try adding small cards now
  while (result.size() < numCards) {
    if (lowDeck.size() == 0) {
      throw bridge_exception("hand not filled, no low cards left");
    }
    int rand = random() % lowDeck.size();
    ++nrands;
    SUIT s;
    RANK r;
    if (!getIdxCard(lowDeck, rand, &s, &r)) {
      throw bridge_exception("card for idx not found");
    }

    result.addCard(s, r);
    if (!shape.holds(result)) {
      result.removeCard(s, r);
      lowDeck.removeCard(s, r);  // this card is useless
      continue;                  // try again
    }
    prob /= lowDeck.size();
    lowDeck.removeCard(s, r);
  }

  return make_pair(prob, result);
}

SampleReport SampleHandsErnie::sampleNT(long int seed,
                                        const Hand& pdHand) const {
  srandom(seed);

  nrands = 0;  // global
  PointRange pr(15, 17);
  Balanced bal;
  SumValue<double> stats;

  int tot = 0, nt = 0;
  while (nt < 100) {
    ++tot;
    try {
      pair<double, Hand> h = getConstrained(pdHand, pr, bal);
      stats.add(h.first);
      ++nt;
    } catch (bridge_exception& be) {
      // skip
    }
  }

  //  stats.printAvgStd(cout);
  return SampleReport(tot, nt, nrands);
}

}  // namespace dtac
