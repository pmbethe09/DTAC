package edu.nyu.bridge.scoring;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.util.Contract;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class ScorerTest {
  @Test
  public void testBasicScores() {
    assertThat(Scorer.score(Contract.of(8, Suit.CLUBS, Direction.SOUTH), Bridge.Vulnerability.NONE,
                   Result.total(0)))
        .named("2C down 6")
        .isEqualTo(Score.of(Direction.SOUTH, -400));
    assertThat(Scorer.score(Contract.of(9, Suit.SPADES, Direction.SOUTH), Bridge.Vulnerability.NONE,
                   Result.total(9)))
        .named("3S=")
        .isEqualTo(Score.of(Direction.SOUTH, 140));
    assertThat(Scorer.score(Contract.of(11, Suit.CLUBS, Direction.WEST), Bridge.Vulnerability.NONE,
                   Result.total(12)))
        .named("5c+1")
        .isEqualTo(Score.of(Direction.WEST, 420));
    assertThat(Scorer.score(Contract.of(10, Suit.SPADES, Direction.EAST), Bridge.Vulnerability.EW,
                   Result.total(12)))
        .named("4s+2")
        .isEqualTo(Score.of(Direction.EAST, 680));
    assertThat(Scorer.score(Contract.parse("4SXS"), Bridge.Vulnerability.BOTH, Result.total(8)))
        .named("4SX-2 vul")
        .isEqualTo(Score.of(Direction.SOUTH, -500));
    assertThat(Scorer.score(Contract.parse("1NXXN"), Bridge.Vulnerability.NONE, Result.total(8)))
        .named("1NXX+1")
        .isEqualTo(Score.of(Direction.NORTH, 760));
  }
}