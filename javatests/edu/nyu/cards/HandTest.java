package edu.nyu.cards;

import static com.google.common.truth.Truth.assertThat;

import com.google.common.collect.ImmutableMap;
import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;
import java.util.EnumSet;
import org.junit.Test;

public class HandTest {
  @Test
  public void testDeck() {
    assertThat(52).isEqualTo(Hand.deck().size());
  }

  @Test
  public void testThereAndBack() {
    Hand hand = Hand.fromString("AK43.KT7.Q.AJ975");
    assertThat(13).isEqualTo(hand.size());
    assertThat(hand).isEqualTo(Hand.fromProto(hand.toProto()));
  }

  @Test
  public void testParse() {
    Hand hand = Hand.fromString("AK43.KT7.Q.AJ975");
    ImmutableMap<Suit, EnumSet<Rank>> expected =
        ImmutableMap.of(
            Suit.SPADES,
            EnumSet.of(Rank.ACE, Rank.KING, Rank.FOUR, Rank.THREE),
            Suit.HEARTS,
            EnumSet.of(Rank.KING, Rank.TEN, Rank.SEVEN),
            Suit.DIAMONDS,
            EnumSet.of(Rank.QUEEN),
            Suit.CLUBS,
            EnumSet.of(Rank.ACE, Rank.JACK, Rank.NINE, Rank.SEVEN, Rank.FIVE));
    assertThat(expected).isEqualTo(hand.cards);
  }

  @Test
  public void testSubset() {
    Hand hand = Hand.fromString("AK43.KT7.Q.AJ975");
    assertThat(hand.hasAll(hand)).isTrue();
    assertThat(hand.hasAll(Hand.fromString("A43.K7.Q.A75"))).isTrue();
    // added stray SQ
    assertThat(hand.hasAll(Hand.fromString("AQ43.K7.Q.A75"))).isFalse();
  }
}
