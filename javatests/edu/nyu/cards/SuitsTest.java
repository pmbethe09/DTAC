package edu.nyu.cards;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class SuitsTest {
  @Test
  public void testSuits() {
    assertThat(Suit.CLUBS).isEqualTo(Suits.char2Suit('c'));
    assertThat(Suit.DIAMONDS).isEqualTo(Suits.char2Suit('D'));
    assertThat(Suit.SPADES).isEqualTo(Suits.char2Suit('S'));
    assertThat('H').isEqualTo(Suits.suit2Char(Suits.char2Suit('H')));
  }
}
