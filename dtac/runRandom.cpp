
#include <stdlib.h>

#include <iostream>

#include "dtac/Hand.h"
#include "dtac/SampleHands.h"

using dtac::CLUBS;
using dtac::NOTRUMP;
using dtac::SUIT;
using dtac::WEST;

using dtac::AnyShape;
using dtac::Balanced;
using dtac::bridge_exception;
using dtac::Hand;
using dtac::IConstraint;
using dtac::PointRange;
using dtac::SampleHandsBase;
using dtac::SampleHandsErnie;
using dtac::SampleHandsRandom;
using dtac::SampleReport;
using dtac::Unbalanced;

using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::vector;

static void dump(const string &name, const SampleReport &all, int size) {
  cout << name << " tot=" << all.total / (double)size
       << " hits=" << all.matches / (double)size
       << " rands=" << all.randomsNeeded / (double)size
       << " rate=" << all.matches / (double)all.total << endl;
}

static void dorun(const string &name, const Hand &pdHand,
                  const SampleHandsBase &sampler) {
  SampleReport all;
  const int size = 1000;
  for (int seed = 1; seed <= size; ++seed) {
    all += sampler.sampleNT(seed, pdHand);
  }
  dump(name, all, size);
}

// one hand, then the other
static void dorun2(const string &name,
                   const vector<const IConstraint *> &constraints,
                   const SampleHandsBase &sampler) {
  SampleReport all;
  const int size = 1000;
  for (int seed = 1; seed <= size; ++seed) {
    Hand pdHand = SampleHandsBase::getConstrainedRandom(Hand(), constraints);
    all += sampler.sampleNT(seed, pdHand);
  }
  dump(name, all, size);
}

static int sampleTest(const SampleHandsBase &shr) {
  dorun("all", Hand(), shr);
  Hand hand(Hand::fromString("aj52.743.kt82.q7"));
  dorun("10cnt", hand, shr);
  Balanced bal;
  PointRange pc(10, 10);
  vector<const IConstraint *> vec;
  vec.push_back(&bal);
  vec.push_back(&pc);
  dorun2("10cntbal", vec, shr);

  pc = PointRange(18, 20);
  dorun2("18-20cntbal", vec, shr);

  Unbalanced unbal;
  vec[0] = &unbal;
  pc = PointRange(15, 17);
  dorun2("15-17cntunbal", vec, shr);
  pc = PointRange(10, 10);
  dorun2("10cntunbal", vec, shr);

  // dorun("ernie", hand, she);
  return 0;
}

int main(int argc, char **argv) {
  if (argc > 1 && !strncmp(argv[1], "sample", 7)) {
    return sampleTest(SampleHandsRandom());
  }

  Hand hand(Hand::fromString("aj52.743.kt82.q7"));
  Balanced bal;
  PointRange pc(15, 17), anyPr(0, 40);
  SampleHandsErnie shr;
  vector<Hand> allHands(4);

  try {
    for (int i = 0; i < 10; ++i) {
      cout << "me:" << hand.toString() << endl;
      Hand pard = shr.getConstrained(hand, pc, bal).second;
      cout << "pard:" << pard.toString() << endl;
      Hand lho = shr.getConstrained(hand + pard, anyPr, AnyShape()).second;
      cout << "lho:" << lho.toString() << endl;
      Hand rho = Hand::complement(hand, pard, lho);
      cout << "rho:" << rho.toString() << endl;
      allHands[0] = rho;
      allHands[1] = hand;
      allHands[2] = lho;
      allHands[3] = pard;
    }
  } catch (bridge_exception &be) {
    cerr << be.what() << endl;
    return 1;
  }
  return 0;
}
