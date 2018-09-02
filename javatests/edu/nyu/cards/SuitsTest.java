package edu.nyu.cards;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class SuitsTest {
  @Test
  public void testSuits() {
    assertThat(Suit.CLUBS).named("clubs").isEqualTo(Suits.char2Suit('c'));
    assertThat(Suit.DIAMONDS).named("diamonds").isEqualTo(Suits.char2Suit('D'));
    assertThat(Suit.SPADES).named("spades").isEqualTo(Suits.char2Suit('S'));
    assertThat('H').named("2convert").isEqualTo(Suits.suit2Char(Suits.char2Suit('H')));
  }
}
