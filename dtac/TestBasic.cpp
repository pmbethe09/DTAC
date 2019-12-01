#include <gtest/gtest.h>
#include <stdarg.h>

#include "dtac/BridgeTestBase.h"
#include "dtac/Claim.h"
#include "dtac/WorstSearch.h"

using namespace dtac;

using std::string;

#define ClaimSP std::shared_ptr<Claim>
#define LoseATrickSP std::shared_ptr<LoseATrick>

#define ASSERT_SP(sp) ASSERT_TRUE(sp.get())

class InspectWorstSearch : public WorstSearch {
 public:
  void setUp() { initDefender(); }
};

TEST(WhiteBoxTest, runTest) {
  InspectWorstSearch iws;
  iws.setUp();
  ASSERT_GT(iws.defender.length[0], 0);

  ClaimTrick(Play(SPADES, JACK), Play(SPADES, THREE), SPADES, false);
}

static ClaimSP runOne(const char *lead, const char *follow, const char *dfndr,
                      SUIT trumps = NOTRUMP,
                      const CombinedDefender *constraints = NULL,
                      int tricks = -1, bool oppsLead = false,
                      SUIT oppsSuit = NOTRUMP) {
  Hand ld = Hand::fromString(lead);
  Hand fol = Hand::fromString(follow);
  Hand df = Hand::fromString(dfndr);
  return BridgeTestBase::runOne(ld, fol, df, trumps, constraints, tricks,
                                oppsLead, oppsSuit);
}

static ClaimSP runOne(const Hand &ld, const Hand &fol, const Hand &df,
                      SUIT trumps = NOTRUMP,
                      const CombinedDefender *constraints = NULL,
                      int tricks = -1, bool oppsLead = false,
                      SUIT oppsSuit = NOTRUMP) {
  return BridgeTestBase::runOne(ld, fol, df, trumps, constraints, tricks,
                                oppsLead, oppsSuit);
}

TEST(BlackBoxTestClaimK, winIfOpps) {
  const char *hands[] = {"A.A", "..22", "T.T"};
  // we claim if we are on lead
  // opps can only play a major, so we always win
  ClaimSP c =
      runOne(hands[0], hands[1], hands[2], NOTRUMP, NULL /*constr*/, 2, true);
  ASSERT_SP(c);

  // can't claim K when K>N
  c = runOne(hands[0], hands[1], hands[2], NOTRUMP, NULL /*constr*/, 3, true);
  ASSERT_TRUE(!c);
}

TEST(BlackBoxTestClaimK, loseIfOpps) {
  const char *hands[] = {"AK.AK", "2.2.2.2", "T.T.T.T"};
  // we claim if we are on lead
  ClaimSP c = runOne(hands[0], hands[1], hands[2], NOTRUMP, NULL /*constr*/, 4,
                     false, CLUBS);
  ASSERT_SP(c);

  // can't claim if the opps lead though, they can cash a minor.
  c = runOne(hands[0], hands[1], hands[2], NOTRUMP, NULL /*constr*/, 4, true,
             CLUBS);
  ASSERT_TRUE(!c);
  c = runOne(hands[0], hands[1], hands[2], NOTRUMP, NULL /*constr*/, 4, true);
  ASSERT_TRUE(!c);

  const char *hands2[] = {"AK.A", "..232", "T.T.T"};
  // we claim if we are on lead
  // but opps can only play a diamond
  c = runOne(hands2[0], hands2[1], hands2[2], NOTRUMP, NULL /*constr*/, 3,
             true);
  ASSERT_TRUE(!c);
}

TEST(BlackBoxTestClaimK, classic) {
  const char *hands[] = {"A.A.A.KQJ", "2.2.32.32", "KQ.KQJ.KQJ.A65"};
  // we claim '5' if we are on lead
  ClaimSP c =
      runOne(hands[0], hands[1], hands[2], NOTRUMP, NULL /*constr*/, 5, false);
  ASSERT_SP(c);
  // c->printDot("classic.dot");
}

TEST(BlackBoxTestClaimK, classic2) {
  const char *hands[] = {"QJTAKAKA", "43232322", "AK7.QJT9.QJT9.K"};
  // we claim '6' if we are on lead, double drive-out
  ClaimSP c =
      runOne(hands[0], hands[1], hands[2], NOTRUMP, NULL /*constr*/, 6, false);

  // since we only support a 1-trick loss at this time
  ASSERT_TRUE(!c);
}

TEST(BlackBoxTestClaimK, problems) {
  const char *hands[] = {"..AK.AJ8", "7...K743", "J98.JT987.987.QT962"};
  ClaimSP c =
      runOne(hands[0], hands[1], hands[2], CLUBS, NULL /*constr*/, 4, false);

  ASSERT_TRUE(!c) << "should fail to find a soln w/o constr";

  CombinedDefender cd;
  // only assumption is trumps 3-2
  cd.read(string("S: (J, 3, 0); H: (J, 9, 0); D: (9, 5, 0); C: (Q, 3, 2)"),
          hands[2]);
  c = runOne(hands[0], hands[1], hands[2], CLUBS, &cd /*constr*/, 4, false);

  ASSERT_SP(c) << "should find a soln w 3-2 trumps";
  // c->printDot("trumps32sml.dot");
}

// copy in Hand
static void workBackLoser(const Hand &leadin, const Hand &followin,
                          const Hand &dfndr, SUIT trumps,
                          const CombinedDefender &baseCd, const Claim &claim) {
  ASSERT_SP(claim.lostTrick) << "only claims with losers allowed";
  const int tricks = claim.toTake;
  const int size = claim.tricks.size();
  ClaimSP csp;

  for (int toGo = 0; toGo <= size; ++toGo) {
    // not sure about undo - just copy
    Hand lead(leadin), follow(followin);
    CombinedDefender cd(baseCd);

    bool stillOnLead = true;  // lead may change hands
    int toApply = size - toGo;
    // apply this many tricks and see if we find shorter guy
    if (toApply > 0)
      BridgeTestBase::applyTricks(toApply, claim, stillOnLead, lead, follow,
                                  trumps, cd);

    const int subTake = tricks - toApply;
    if (toGo > 0) {
      csp = runOne(stillOnLead ? lead : follow, stillOnLead ? follow : lead,
                   dfndr, trumps, &cd, subTake);
    } else {
      // deal with the lose a trick and family
      ClaimSP tmp = ClaimSP(new Claim(claim.toTake));
      tmp->lostTrick = LoseATrickSP(new LoseATrick);
      LoseATrick &result = *tmp->lostTrick;

      LoseATrick &loser = *claim.lostTrick;
      if (loser.theyDuck) {
        ASSERT_GE(loser.theyDuck->toTake, subTake)
            << "at least matching toTake";
        ClaimSP tD = loser.theyDuck;
        if (tD->lostTrick) {
          workBackLoser(lead, follow, dfndr, trumps, cd, *tD);
        } else {
          bool dmyLeads = !(claim.tricks[0].leadWins);
          BridgeTestBase::workBack(dmyLeads ? follow : lead,
                                   dmyLeads ? lead : follow, dfndr, trumps, cd,
                                   *tD);
        }

        result.theyDuck = loser.theyDuck;
      }

      for (int i = 0; i < NUM_SUITS; ++i) {
        if (loser.ruffSuitClaims[i]) {
          // opp ruffs, adjust
          CombinedDefender adjCd(cd);
          ASSERT_GT(adjCd.length[trumps], 0) << "cd must have trumps to ruff";
          if (adjCd.min_length[trumps] > 0)
            --adjCd.min_length[trumps];
          else
            --adjCd.length[trumps];
          Claim &ruffClaim = *loser.ruffSuitClaims[i];
          ASSERT_EQ(subTake + 1, ruffClaim.toTake) << "take what I expect";
          BridgeTestBase::workBack(lead, follow, dfndr, trumps, adjCd,
                                   ruffClaim, (SUIT)i);
          result.ruffSuitClaims[i] =
              loser.ruffSuitClaims[i];  // if we get here, copy out
        }
        if (loser.winSuitClaims[i]) {
          // opps take cheaply, adjust
          Claim &suitClaim = *loser.winSuitClaims[i];
          CombinedDefender adjCd(cd);
          Hand newDfndr(dfndr);
          const ClaimTrick &trick =
              *claim.tricks.rbegin();  // last trick of claim!
          ASSERT_EQ(trick.winner->suit, trick.lead.suit)
              << "no ruff on a 'won' trick";
          SUIT suit = trick.winner->suit;
          RANK r = trick.winner->winCheaply(dfndr);
          bool haveWinner = r <= adjCd.highest[suit];
          if (!haveWinner) {
            ASSERT_LE(r, adjCd.highest[suit]) << "win with a card we hold";
          }
          newDfndr.removeCard(suit, r);
          if (adjCd.length[suit] > 0) {  // still length in the suit after apply
            adjCd.origHolding.removeCard(suit, r);
            if (r == adjCd.highest[suit]) {  // drove highest!
              adjCd.highest[suit] = adjCd.origHolding.getHighestInSuit(suit);
            }
          }
          BridgeTestBase::workBack(lead, follow, newDfndr, trumps, adjCd,
                                   suitClaim, (SUIT)i);
          result.winSuitClaims[i] =
              loser.winSuitClaims[i];  // if we get here, copy out
        }
      }
      csp = tmp;  // copy out
    }

    ASSERT_SP(csp) << "Loser-backtrack '-" << toApply << "' failed";

    // assert matching
    int offset = claim.tricks.size() - csp->tricks.size();  // for shorter guys
    for (int i = 0; i < csp->tricks.size(); ++i) {
      const ClaimTrick &ctOrig = claim.tricks[i + offset];
      const ClaimTrick &ct = csp->tricks[i];
      ASSERT_EQ(ctOrig, ct)
          << "backtrack '-" << toApply << "' trick " << i << " did not match.";
    }  // end for each trick to match

  }  // end for toGo
}

/// helper fn - work back and include loser...
static void workBackLoser(const char *hands[3], SUIT trumps,
                          const CombinedDefender &baseCd, const Claim &claim) {
  Hand leadin = Hand::fromString(hands[0]),
       followin = Hand::fromString(hands[1]),
       dfndr = Hand::fromString(hands[2]);
  workBackLoser(leadin, followin, dfndr, trumps, baseCd, claim);
}

TEST(BlackBoxTestClaimK, driveOut) {
  // from test your timing
  // concede an unneccessary trick to the JD to force an entry to dummies AK of
  // H (1 for 2)
  const char *hands[] = {"A65..AKQT9732.A", "QT72.AK32.86.43",
                         "KJ9843.QJT987654.J54.JT98765"};
  CombinedDefender cd;
  // only assumption is no-one ruffs in after 1 rnd of trump, and opps have 2
  // hearts
  cd.read(string("S: (K, 5, 1); H: (Q, 7, 2); D: (J, 3, 0); C: (J, 5, 1)"),
          hands[2]);

  Claim myClaim(11);
  BridgeTestBase::parseClaim("D2 D6", myClaim, DIAMONDS);
  myClaim.lostTrick = LoseATrickSP(new LoseATrick);
  LoseATrickSP lost = myClaim.lostTrick;
  lost->theyDuck = ClaimSP(new Claim(11));  // since I know we take the rest
  // They duck - we win, cash 2 hearts and run tricks.
  BridgeTestBase::parseClaim(
      "HK S5, HA S6, D8 DQ, DK C3, D3 C4, D7 H2, D9 H3, DT S2, DA S7, SA ST, "
      "CA SQ",
      *lost->theyDuck, DIAMONDS, HEARTS);

  // they rise J and ...
  lost->winSuitClaims[CLUBS] = ClaimSP(new Claim(11));
  BridgeTestBase::parseClaim(
      "CA C3, D3 D8, HK S5, HA S6, C4 D7, D9 H2, DT H3, DQ S2, DK S7, DA ST, "
      "SA SQ",
      *lost->winSuitClaims[CLUBS], DIAMONDS);
  lost->winSuitClaims[DIAMONDS] = ClaimSP(new Claim(11));
  BridgeTestBase::parseClaim(
      "D3 D8, HK S5, HA S6, C3 CA, D7 C4, D9 H2, DT H3, DQ S2, DK S7, DA ST, "
      "SA SQ",
      *lost->winSuitClaims[DIAMONDS], DIAMONDS);
  lost->winSuitClaims[HEARTS] = ClaimSP(new Claim(11));
  BridgeTestBase::parseClaim(
      "S5 HK, D8 D3, HA S6, C3 CA, D7 C4, D9 H2, DT H3, DQ S2, DK S7, DA ST, "
      "SA SQ",
      *lost->winSuitClaims[HEARTS], DIAMONDS, HEARTS);
  lost->winSuitClaims[SPADES] = ClaimSP(new Claim(11));
  BridgeTestBase::parseClaim(
      "SA S2, D3 D8, HK S5, HA S6, C3 CA, D7 C4, D9 H2, DT H3, DQ S7, DK ST, "
      "DA SQ",
      *lost->winSuitClaims[SPADES], DIAMONDS);

  workBackLoser(hands, DIAMONDS, cd, myClaim);

  ClaimSP c =
      runOne(hands[0], hands[1], hands[2], DIAMONDS, &cd /*constr*/, 11, false);

  ASSERT_SP(c) << "should find a soln by driving the diamond";
  // c->printDot("testYP.dot");
}

TEST(BlackBoxTestClaimK, wolff) {
  const char *hands0[] = {"Q32.AK74.Q2.QJT9", "AKJ654.5.AT642.A",
                          "T987.QJT98632.KJ9875.K8765432"};
  CombinedDefender cd0;
  // assumption: Heart lead, trumps 3-1, clubs try both 5-3/4-4
  cd0.read(string("S: (J, 3, 0); H: (J, 9, 0); D: (9, 5, 0); C: (Q, 1, 0)"),
           hands0[2]);
  ClaimSP c = runOne(hands0[0], hands0[1], hands0[2], CLUBS, &cd0, 8, false);
}

TEST(BlackBoxTestClaimK, masterTrump) {
  const char *hands1[] = {"2.A", ".2.2", "A..AKQ"};
  // I lead hearts till they ruff, I  claim
  ClaimSP c =
      runOne(hands1[0], hands1[1], hands1[2], SPADES, NULL /*CD*/, 1, false);
  ASSERT_SP(c) << "simplest they ruff fails";

  //    const char * hands3[] = {"2.AK", ".2.2.4", "A..AKQJT.AKQJ"};
  const char *hands3[] = {"2.AK", ".2.2.4", "A..AKQ.AK"};
  // I lead hearts till they ruff, I  claim
  c = runOne(hands3[0], hands3[1], hands3[2], SPADES, NULL /*CD*/, 2, false);
  ASSERT_SP(c) << "simple they ruff w/ master fails";

  const char *hands4[] = {"2.AK", ".2.2.4", "AK..AK.AK"};
  // I lead hearts till they ruff, I  claim
  c = runOne(hands4[0], hands4[1], hands4[2], SPADES, NULL /*CD*/, 2, false);

  ASSERT_TRUE(!c) << "they have 2 trumps yet I didn't fail";

  const char *hands0[] = {".AKQ.KJT43.J", "765.2.AQ5.74",
                          "J98.JT9876543.98762.Q"};
  CombinedDefender cd0;
  // only assumption is trumps 3-2
  cd0.read(string("S: (J, 3, 0); H: (J, 9, 0); D: (9, 5, 0); C: (Q, 1, 0)"),
           hands0[2]);
  c = runOne(hands0[0], hands0[1], hands0[2], CLUBS, &cd0, 8, false);

  // draw 2 trump, opps ruff in when they want to
  ASSERT_SP(c) << "complicated they have master failed";

  const char *hands[] = {".AKQ.KJT43.AJ8", "765.2.AQ5.K743",
                         "J98.JT9876543.98762.QT962"};
  CombinedDefender cd;
  // only assumption is trumps 3-2
  cd.read(string("S: (J, 3, 0); H: (J, 9, 0); D: (9, 5, 0); C: (Q, 3, 2)"),
          hands[2]);
  c = runOne(hands[0], hands[1], hands[2], CLUBS, NULL /*constr*/, 10, false);

  //    if (c)
  // c->print(std::cout);

  ASSERT_TRUE(!c) << "should fail to find a soln w/o constr";

  c = runOne(hands[0], hands[1], hands[2], CLUBS, &cd, 10, false);
  // draw 2 trump, opps ruff in when they want tos
  ASSERT_SP(c) << "Draw AK of trump and claim with master-trump loser failed";
  // c->printDot("trumps32.dot");
}

static void test2Way(const char *hStr, int expectMask[]) {
  Hand h(Hand::fromString(string(hStr)));
  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(h.getSuit(i), expectMask[i]);
  }
}

static void test2Way(const char *hStr, int spades, int hearts, int diamonds,
                     int clubs) {
  int expect[4] = {clubs, diamonds, hearts, spades};
  test2Way(hStr, expect);
}

static int mask(int n, ...) {
  va_list args;
  va_start(args, n);
  int result = 0;
  for (int i = 0; i < n; ++i) result |= va_arg(args, int);
  va_end(args);
  return result;
}

TEST(TestHand, readWrite) {
  test2Way("AKQ", mask(3, ACE, KING, QUEEN), 0, 0, 0);
  test2Way("432", mask(3, FOUR, THREE, TWO), 0, 0, 0);
  test2Way("JT9", mask(3, JACK, TEN, NINE), 0, 0, 0);
  test2Way("-A,KQ,J", 0, (int)ACE, mask(2, KING, QUEEN), JACK);
  test2Way("JQKA", JACK, QUEEN, KING, ACE);
  // t2 = Hand::fromString("432");
  // dfndr = Hand::fromString("JT9");
}

TEST(TestHand, testCombinedDefender) {
  CombinedDefender cd;
  cd.read(string("S: (T, 3, 1); H: (Q, 4, 2); D: (J, 5, 3); C: (Q, 6, 0)"),
          "T98.Q432.J9876.Q76543");

  ASSERT_TRUE(
      cd.length[(int)SPADES] == 3 && cd.min_length[(int)SPADES] == 1 &&
      cd.length[(int)HEARTS] == 4 && cd.min_length[(int)HEARTS] == 2 &&
      cd.length[(int)DIAMONDS] == 5 && cd.min_length[(int)DIAMONDS] == 3 &&
      cd.length[(int)CLUBS] == 6 && cd.min_length[(int)CLUBS] == 0 &&
      cd.highest[(int)SPADES] == TEN && cd.highest[(int)HEARTS] == QUEEN &&
      cd.highest[(int)DIAMONDS] == JACK && cd.highest[(int)CLUBS] == QUEEN)
      << "CombinedDefender parsing not working";
}
