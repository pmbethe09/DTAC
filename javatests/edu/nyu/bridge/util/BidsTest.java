package edu.nyu.bridge.util;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Level;
import edu.nyu.cards.gen.Cards.Suit;

public class BidsTest {
  @Test
  public void testThereAndBackAgain() {
    assertEquals(Bid.TWO_NOTRUMPS, Bids.bid(Level.TWO, Suit.NOTRUMPS));
    assertEquals(Bid.THREE_SPADES, Bids.bid(Level.THREE, Suit.SPADES));
    assertEquals(Bid.FOUR_HEARTS, Bids.bid(Level.FOUR, Suit.HEARTS));
    assertEquals(Bid.SIX_CLUBS, Bids.bid(Level.SIX, Suit.CLUBS));
  }
}
