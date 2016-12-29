package edu.nyu.cards;

import org.junit.Test;

import edu.nyu.cards.gen.Cards.Suit;
import junit.framework.TestCase;

public class SuitsTest extends TestCase {
  @Test
  public void testSuits() {
    assertEquals("clubs", Suit.CLUBS, Suits.char2Suit('c'));
    assertEquals("diamonds", Suit.DIAMONDS, Suits.char2Suit('D'));
  }
}
