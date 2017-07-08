package edu.nyu.bridge.scoring;

import static org.junit.Assert.assertEquals;

import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.util.Contract;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class ScorerTest {
  @Test
  public void testBasicScores() {
    assertEquals("2C down 6",
        Scorer.score(Contract.of(8, Suit.CLUBS, Direction.SOUTH), Bridge.Vulnerability.NONE,
            Result.total(0)),
        Score.of(Direction.SOUTH, -400));
    assertEquals("3S=",
        Scorer.score(Contract.of(9, Suit.SPADES, Direction.SOUTH), Bridge.Vulnerability.NONE,
            Result.total(9)),
        Score.of(Direction.SOUTH, 140));
    assertEquals("5c+1",
        Scorer.score(Contract.of(11, Suit.CLUBS, Direction.WEST), Bridge.Vulnerability.NONE,
            Result.total(12)),
        Score.of(Direction.WEST, 420));
    assertEquals("4s+2",
        Scorer.score(Contract.of(10, Suit.SPADES, Direction.EAST), Bridge.Vulnerability.EW,
            Result.total(12)),
        Score.of(Direction.EAST, 680));
    assertEquals("4SX-2 vul",
        Scorer.score(Contract.parse("4SXS"), Bridge.Vulnerability.BOTH, Result.total(8)),
        Score.of(Direction.SOUTH, -500));
    assertEquals("1NXX+1",
        Scorer.score(Contract.parse("1NXXN"), Bridge.Vulnerability.NONE, Result.total(8)),
        Score.of(Direction.NORTH, 760));
  }
}