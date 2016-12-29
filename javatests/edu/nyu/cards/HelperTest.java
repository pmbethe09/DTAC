package edu.nyu.cards;

import org.junit.Test;

import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;
import junit.framework.TestCase;

/**
 * Test all the helper classes {@link Cards}, {@link Ranks}, {@link Suits}
 *
 * @author pbethe
 */
public class HelperTest extends TestCase {
  @Test
  public void testCards() {
    assertEquals("convert", "H2", Cards.card2String(Cards.string2Card("H2")));
  }

  @Test
  public void testRanks() {
    assertEquals("2convertRank", Rank.KING, Ranks.char2Rank('K'));
    assertEquals("Seven", '7', Ranks.rank2Char(Ranks.char2Rank('7')));
  }

  @Test
  public void testSuits() {
    assertEquals("spades", Suit.SPADES, Suits.char2Suit('S'));
    assertEquals("2convert", 'H', Suits.suit2Char(Suits.char2Suit('H')));
  }
}
