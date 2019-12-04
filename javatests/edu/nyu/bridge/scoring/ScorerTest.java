package edu.nyu.bridge.scoring;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.util.Contract;
import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public class ScorerTest {
  @Test
  public void testFromContract() {
    Contract contract = Contract.of(8, Suit.CLUBS, Direction.SOUTH);
    assertThat(ContractResult.making(contract).score(Bridge.Vulnerability.NONE))
        .isEqualTo(ContractScore.of(contract, Score.of(Direction.SOUTH, 90)));
  }

  @Test
  public void testBasicScores() {
    assertThat(
            Scorer.score(
                Contract.of(8, Suit.CLUBS, Direction.SOUTH),
                Bridge.Vulnerability.NONE,
                Result.total(0)))
        .isEqualTo(Score.of(Direction.SOUTH, -400));
    assertThat(
            Scorer.score(
                Contract.of(9, Suit.SPADES, Direction.SOUTH),
                Bridge.Vulnerability.NONE,
                Result.total(9)))
        .isEqualTo(Score.of(Direction.SOUTH, 140));
    assertThat(
            Scorer.score(
                Contract.of(11, Suit.CLUBS, Direction.WEST),
                Bridge.Vulnerability.NONE,
                Result.total(12)))
        .isEqualTo(Score.of(Direction.WEST, 420));
    assertThat(
            Scorer.score(
                Contract.of(10, Suit.SPADES, Direction.EAST),
                Bridge.Vulnerability.EW,
                Result.total(12)))
        .isEqualTo(Score.of(Direction.EAST, 680));
    assertThat(Scorer.score(Contract.parse("4SXS"), Bridge.Vulnerability.BOTH, Result.total(8)))
        .isEqualTo(Score.of(Direction.SOUTH, -500));
    assertThat(Scorer.score(Contract.parse("1NXXN"), Bridge.Vulnerability.NONE, Result.total(8)))
        .isEqualTo(Score.of(Direction.NORTH, 760));
  }
}
