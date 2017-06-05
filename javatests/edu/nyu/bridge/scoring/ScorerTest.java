package edu.nyu.bridge.scoring;

import static org.junit.Assert.assertEquals;

import edu.nyu.bridge.util.Contract;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class ScorerTest {
  @Test
  public void testPbnParse() {
    assertEquals(
        "same both ways", Scorer.score(Contract.of(8, Suit.CLUBS, Direction.SOUTH), null, 0), 42);
  }
}