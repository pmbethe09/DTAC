package edu.nyu.cards;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.cards.gen.Cards.Card;
import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class CardsTest {
  @Test
  public void testConvert() {
    assertThat("H2").named("convert").isEqualTo(Cards.card2String(Cards.string2Card("H2")));
  }

  @Test
  public void testEnum() {
    assertThat(Card.newBuilder().setRank(Rank.TWO).setSuit(Suit.HEARTS).build())
        .named("parse")
        .isEqualTo(Cards.string2Card("H2"));
  }
}
