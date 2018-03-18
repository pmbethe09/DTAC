package edu.nyu.cards;

import static com.google.common.truth.Truth.assertThat;

import java.util.EnumSet;

import org.junit.Test;

import com.google.common.collect.ImmutableMap;

import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;

public class HandTest {
  @Test
  public void testDeck() {
    assertThat(52).named("52 cards").isEqualTo(Hand.deck().size());
  }

  @Test
  public void testThereAndBack() {
    Hand hand = Hand.fromString("AK43.KT7.Q.AJ975");
    assertThat(13).named("13 card parse").isEqualTo(hand.size());
    assertThat(hand).named("converted equals").isEqualTo(Hand.fromProto(hand.toProto()));
  }

  @Test
  public void testParse() {
    Hand hand = Hand.fromString("AK43.KT7.Q.AJ975");
    ImmutableMap<Suit, EnumSet<Rank>> expected = ImmutableMap.of(Suit.SPADES,
        EnumSet.of(Rank.ACE, Rank.KING, Rank.FOUR, Rank.THREE), Suit.HEARTS,
        EnumSet.of(Rank.KING, Rank.TEN, Rank.SEVEN), Suit.DIAMONDS, EnumSet.of(Rank.QUEEN),
        Suit.CLUBS, EnumSet.of(Rank.ACE, Rank.JACK, Rank.NINE, Rank.SEVEN, Rank.FIVE));
    assertThat(expected).named("expected parse").isEqualTo(hand.cards);
  }
}
