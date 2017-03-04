#include "dtac/BridgeTestBase.hpp"

#include <gtest/gtest.h>

#include "dtac/Claim.hpp"

using namespace dtac;

using std::cout;
using std::endl;
using std::string;

#define runOne BridgeTestBase::runOne
#define runOneConstr BridgeTestBase::runOneConstr

#define ASSERT_SP(sp) ASSERT_TRUE(sp.get())

// hand from mom - where you can combine chances...
TEST(BlackBoxTest, eitherOr) {
  // 3=8=1=1
  const char* hands[] = {
      "Q32.AJ876543..",
      "AKJT94.K.J7.QJ"}; /* A of clubs, then (luckily) diamond to the Q, Ace
                            (so decl on lead) */
  // no claim if both 3-1
  const char* cdef0 = "S: (8, 3, 1); H: (Q, 3, 1); D: (K, 7, 0); C: (K, 7, 0)";
  ClaimSP c = runOneConstr(hands[1], hands[0], SPADES, cdef0);
  ASSERT_TRUE(!c);

  // if spades 2-2, H 3-1
  const char* cdef = "S: (8, 2, 2); H: (Q, 3, 1); D: (K, 7, 0); C: (K, 7, 0)";
  c = runOneConstr(hands[1], hands[0], SPADES, cdef);
  ASSERT_SP(c);
  //    c->printDot("distS22.dot");

  cout << endl;
  // c->print(std::cout);
  cout << endl;
  // or spades 3-1, H 2-2 :: how to combine these two chances?
  const char* cdef1 = "S: (8, 3, 1); H: (Q, 2, 2); D: (K, 7, 0); C: (K, 7, 0)";
  c = runOneConstr(hands[1], hands[0], SPADES, cdef1);
  ASSERT_SP(c);
  //   c->printDot("distS31.dot");
  //    c->print(std::cout);
}

// from dad
TEST(BlackBoxTest, unblockGrand) {
  // spade led, making it unable to reach hand
  const char* hands[] = {"Q.KQJ.6.JT97654", ".A.AKQT9872.AKQ"};
  const char* cdef0 = "S: (J, 8, 0); H: (T, 9, 0); D: (J, 4, 0); C: (8, 3, 0)";
  ClaimSP c = runOneConstr(hands[0], hands[1], NOTRUMP, cdef0);
  ASSERT_SP(c);
  // double unblock
  // c->print(std::cout);

  const char* hands2[] = {"Q.KQJ.6.J976543", ".A.AKQT9872.AKQ"};
  const char* cdef1 = "S: (J, 8, 0); H: (T, 9, 0); D: (J, 4, 0); C: (T, 3, 0)";
  // nope, diamonds could be 4-0, and T of clubs may not be stiff
  c = runOneConstr(hands2[0], hands2[1], NOTRUMP, cdef1);
  ASSERT_TRUE(!c);
}

// from the Blue Ribbons: http://www.acbl.org/nabc/bulletins/2009/03/7/
// page 6
TEST(BlackBoxTest, acblBRGrand) {
  const char* hands[] = {
      "K2.AQ2.75.KJ743",
      "A65.KJ97653.J.A"}; /* diamond led to dummy, won by Ace */
  // take all with trumps 2-1
  const char* cdef = "S: (Q, 8, 0); H: (T, 2, 1); D: (K, 6, 0); C: (Q, 7, 0)";

  ClaimSP c =
      runOne(hands[0], hands[1], HEARTS);  // no claim unless trumps are 2-1,
  ASSERT_TRUE(!c);

  c = runOneConstr(
      hands[0], hands[1], HEARTS,
      cdef);  // if they are, a spade ruff is assured, plus K of C to pitch D
  ASSERT_SP(c);
}

// TODO FIXME
// from http://www.acbl.org/nabc/bulletins/2009/03/7/
// page 4 - after timing, assuming defender ruffs lead up to DK
void timing() {
  // const char * hands[] = {"Q96.QJ..A6", "5.A.KT.873"};
  // const char * cdef = "S: (2, 0, 0); H: (K, 2, 0); D: (Q, 4, 0); C: (K, 8,
  // 0)";
}

TEST(BlackBoxTest, grandIfGuessDistrib) {
  // opps lead club!
  const char* hands[] = {
      "AKQT762.9.A5.J5",
      "93.AK852.QJ984."}; /* first round of clubs, won in dummy, so dummy on
                             lead to t 12 */
  // TODO
  // make 7S on club lead IFF H 5-2 (trumps3-1), and D 3-3
  const char* cdef = "S: (J, 3, 1); H: (Q, 5, 2); D: (K, 3, 3); C: (K, 6, 3)";

  ClaimSP c = runOne(hands[1], hands[0], SPADES);
  ASSERT_TRUE(!c);

  c = runOneConstr(hands[1], hands[0], SPADES, cdef);
  ASSERT_SP(c);

  // OR H 4-3 D 4-2 (but you have to decide!!!)
  const char* cdef2 = "S: (J, 3, 1); H: (Q, 4, 3); D: (K, 4, 2); C: (K, 6, 3)";

  c = runOneConstr(hands[1], hands[0], SPADES, cdef2);
  ASSERT_SP(c);

  // OR, D 4-2 onside, (can't do this)

  // OR H 5-2 onside (can't do this either)
}

// easy grand
// The Aces on Bridge: Thursday, December 10, 2009
TEST(BlackBoxTest, easyGrand) {
  // opps lead a club, ruffed by declarer
  const char* hands[] = {"KQ954.AK9.AK92", "AJ83.854.QJ4.J9"};
  const char* cdef = "S: (T, 3, 0); H: (Q, 7, 0); D: (T, 6, 0); C: (K, 8, 0)";

  ClaimSP c = BridgeTestBase::runOneConstr(hands[0], hands[1], SPADES, cdef);
  ASSERT_TRUE(bool(c));
  // c->printDot("easyGrand.dot");
}

// kantar v2 cover
TEST(BlackBoxTest, kantarV2) {
  const char* hands[] = {"4.K763.AQJ.65432", "AKJ9.AQ9854.53.A"};
  const char* cdef = "S: (Q, 8, 0); H: (J, 2, 1); D: (K, 8, 0); C: (K, 4, 3)";

  CombinedDefender cd;
  cd.read(cdef, Hand::complement(Hand::fromString(hands[0]),
                                 Hand::fromString(hands[1]), Hand()));

  ClaimSP c = BridgeTestBase::runOne(hands[0], hands[1], HEARTS, &cd);
  ASSERT_TRUE(c.get());
  // c->printDot("kantarV2.dot");

  c = BridgeTestBase::runOne(hands[0], hands[1], HEARTS);
  ASSERT_FALSE(c.get());  // can't do it w/o clubs

  // OK change spade Jack to Queen, or diamond Q and of course, claim w/ none.
  string hand0(hands[0]);
  hand0[8] = 'K';
  cd.length[(int)CLUBS] = 7;
  cd.min_length[(int)CLUBS] = 0;

  c = BridgeTestBase::runOne(hand0.c_str(), hands[1], HEARTS, &cd);
  ASSERT_TRUE(c.get());  // 13 winners if trumps 2-1

  string hand1(hands[1]);
  hand1[2] = 'Q';  // now use S queen to pitch D, then ruff. need trumps 2-1

  c = BridgeTestBase::runOne(hands[0], hand1.c_str(), HEARTS, &cd);
  ASSERT_TRUE(c.get());

  // c->print(std::cout);
}

// another 'CHEAPEST_OF' claim
void trumpCoup() {  // decl first?
  // const char * hands[] = {"A8.K9..KQ", "5.T5.J98."};
  // IFF RHO has T6.Q7..95, claim is HK, CKQ (pitch a heart), ruff a heart, coup
}

TEST(BlackBoxTest, goodDistrib) {
  // hand from aces.bridgeblogging.com, 11/19/09
  // K-club lead and won in dummy by the ace (so dummy first
  const char* hands[] = {"KQJ.KT732.A87.J", "A86532.A6.KQ.85",
                         "T974.QJ9854.JT965432.QT9764"};
  ClaimSP c = BridgeTestBase::runOne(hands[0], hands[1], hands[2], SPADES);
  ASSERT_TRUE(!c);  // not possible with all distros

  // verify the claim I see
  Claim claim(12);
  BridgeTestBase::parseClaim(
      "D7 DQ, DK D8, HA H2, S2 SJ, DA H6, H3 S3, S5 SQ, H7 SA, S6 SK, HK C5, "
      "HT C8, CJ S8",
      claim, SPADES);
  ASSERT_EQ(claim.tricks.size(), 12) << "12 trick claim parsed";

  // and now require the opponents to have at least 1S, 2H and 3D
  CombinedDefender cd;
  cd.read(string("S: (T, 3, 1); H: (Q, 4, 2); D: (J, 5, 3); C: (Q, 6, 0)"),
          hands[2]);

  ASSERT_TRUE(claim.verify(cd, SPADES)) << "unable to verify my ordering";
  // below fails (when it should suceed.
  // try working back to fail-level
  BridgeTestBase::workBack(hands, SPADES, cd, claim);

  // if opps were on lead, they could cash a club so...
  c = BridgeTestBase::runOne(hands[0], hands[1], hands[2], SPADES, &cd);
  ASSERT_SP(c) << "find claim w/ constr";
  //    c->printDot("aces.dot");
}

// printer
void printState(const Hand& lead, const Hand& follow, const Hand& dfndr,
                SUIT trumps, const CombinedDefender& constraints) {
  cout << endl << follow.toString() << endl;
  cout << lead.toString() << endl << endl;
  cout << "trumps=" << suit2Char(trumps) << endl;
  cout << "Defender:" << endl << "\t";

  Hand played = Hand::complement(lead, follow, dfndr);
  CombinedDefender cd;
  cd.init(lead, follow, played);
  cd.override(constraints);
  cd.write(cout);
}

TEST(BlackBoxTest, first3) {
  ClaimSP c = runOne("5432", "..4.432", "T9.T.T.", CLUBS, NULL, 3);
  ASSERT_SP(c);
}

TEST(BlackBoxTest, forThesis) {
  ClaimSP c = runOne("AKJ.2", "Q9..2.2", "T.T9.T9.T98", SPADES);
  ASSERT_SP(c);
  //    c->printDot("simpleTrumps.dot");

  c = runOne("A.KQJT", ".A..AKQ2", "T9.T9.T9.QJT9", NOTRUMP);
  ASSERT_SP(c);
  //    c->printDot("simpleExample.dot");
}

TEST(BlackBoxTest, allTrumpsinDummy) {
  ClaimSP c = runOne("432", "---432", "T.T.T.", CLUBS);
  ASSERT_SP(c);
}

TEST(BlackBoxTest, unblock) {
  ClaimSP c = runOne("A,KQJ", "-A,432", "TT98T987");
  ASSERT_SP(c);
  // c->print(std::cout);
}

TEST(BlackBoxTest, simpleAKQ) {
  ClaimSP c = runOne("AKQ", "432", "JT9876");
  ASSERT_SP(c);
  // c->print(std::cout);
}

TEST(BlackBoxTest, failClaim) {
  ClaimSP c = runOne("765", "432", "JT98AK");
  ASSERT_TRUE(!c);
}
