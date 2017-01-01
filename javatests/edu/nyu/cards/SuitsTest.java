package edu.nyu.cards;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import edu.nyu.cards.gen.Cards.Suit;

public class SuitsTest {
  @Test
  public void testSuits() {
    assertEquals("clubs", Suit.CLUBS, Suits.char2Suit('c'));
    assertEquals("diamonds", Suit.DIAMONDS, Suits.char2Suit('D'));
    assertEquals("spades", Suit.SPADES, Suits.char2Suit('S'));
    assertEquals("2convert", 'H', Suits.suit2Char(Suits.char2Suit('H')));
  }
}
