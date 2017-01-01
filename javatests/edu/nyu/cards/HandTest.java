package edu.nyu.cards;

import static org.junit.Assert.assertEquals;

import java.util.EnumSet;

import org.junit.Test;

import com.google.common.collect.ImmutableMap;

import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;

public class HandTest {
  @Test
  public void testDeck() {
    assertEquals("52 cards", 52, Hand.deck().size());
  }

  @Test
  public void testThereAndBack() {
    Hand hand = Hand.fromString("AK43.KT7.Q.AJ975");
    assertEquals("13 card parse", 13, hand.size());
    assertEquals("converted equals", hand, Hand.fromProto(hand.toProto()));
  }

  @Test
  public void testParse() {
    Hand hand = Hand.fromString("AK43.KT7.Q.AJ975");
    ImmutableMap<Suit, EnumSet<Rank>> expected = ImmutableMap.of(Suit.SPADES,
        EnumSet.of(Rank.ACE, Rank.KING, Rank.FOUR, Rank.THREE), Suit.HEARTS,
        EnumSet.of(Rank.KING, Rank.TEN, Rank.SEVEN), Suit.DIAMONDS, EnumSet.of(Rank.QUEEN),
        Suit.CLUBS, EnumSet.of(Rank.ACE, Rank.JACK, Rank.NINE, Rank.SEVEN, Rank.FIVE));
    assertEquals("expected parse", expected, hand.cards);
  }
}
