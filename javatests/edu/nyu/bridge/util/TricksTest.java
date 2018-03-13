package edu.nyu.bridge.util;

import static com.google.common.collect.ImmutableList.toImmutableList;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static edu.nyu.cards.Cards.string2Card;

import java.util.List;

import com.google.common.collect.ImmutableList;
import edu.nyu.cards.Cards;
import org.junit.Test;

import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.Hand;
import edu.nyu.cards.gen.Cards.Card;
import edu.nyu.cards.gen.Cards.Suit;

public class TricksTest {
  @Test
  public void testHigh() {
    Card s5 = string2Card("s5");
    Card sK = string2Card("SK");
    Card hJ = string2Card("hj");
    assertEquals("correct winner", sK, Tricks.high(s5, sK, null));
    assertEquals("correct winner", s5, Tricks.high(s5, hJ, null));
    assertEquals("correct winner", hJ, Tricks.high(s5, hJ, Suit.HEARTS));
    assertEquals("correct winner", s5, Tricks.high(s5, hJ, Suit.CLUBS));
  }

  @Test
  public void testLegal() {
    Hand hand = Hand.fromString("AKQ.KQJ..9753");
    assertTrue("same suit", Tricks.legal(string2Card("SK"), hand, Suit.SPADES));
    assertTrue("discard", Tricks.legal(string2Card("SK"), hand, Suit.DIAMONDS));
    assertFalse("have to follow", Tricks.legal(string2Card("SK"), hand, Suit.HEARTS));
    assertFalse("have to hold it", Tricks.legal(string2Card("H2"), hand, Suit.HEARTS));
  }

  @Test
  public void testWinner() {
    assertEquals("still N", Direction.NORTH,
        Tricks.winner(trick("SK", "S5", "ST", "S9"), Direction.NORTH, null));
    assertEquals("still N", Direction.NORTH,
        Tricks.winner(trick("SK", "S5", "SA", "S9"), Direction.SOUTH, null));
    assertEquals("still N", Direction.NORTH,
        Tricks.winner(trick("SK", "S5", "SA", "D9"), Direction.SOUTH, null));
    assertEquals("E ruffs", Direction.EAST,
        Tricks.winner(trick("SK", "S5", "SA", "D9"), Direction.SOUTH, Suit.DIAMONDS));
    assertEquals("W ruffs so far", Direction.WEST,
        Tricks.winner(trick("SA", "D9"), Direction.SOUTH, Suit.DIAMONDS));
    assertEquals("N overruffs", Direction.NORTH,
        Tricks.winner(trick("SA", "D9", "DJ"), Direction.SOUTH, Suit.DIAMONDS));
    assertEquals("N underruffs", Direction.WEST,
        Tricks.winner(trick("SA", "D9", "D8"), Direction.SOUTH, Suit.DIAMONDS));
  }

  private static List<Card> trick(String... cards) {
    return ImmutableList.copyOf(cards).stream().map(Cards::string2Card).collect(toImmutableList());
  }
}
