package edu.nyu.cards;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import edu.nyu.cards.gen.Cards.Card;
import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;

public class CardsTest {
  @Test
  public void testConvert() {
    assertEquals("convert", "H2", Cards.card2String(Cards.string2Card("H2")));
  }

  @Test
  public void testEnum() {
    assertEquals("parse", Card.newBuilder().setRank(Rank.TWO).setSuit(Suit.HEARTS).build(),
        Cards.string2Card("H2"));
  }
}
