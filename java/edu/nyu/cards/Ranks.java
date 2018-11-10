package edu.nyu.cards;

import com.google.common.base.Converter;
import com.google.common.collect.ImmutableBiMap;
import com.google.common.collect.ImmutableList;
import com.google.common.collect.Lists;
import edu.nyu.cards.gen.Cards.Card;
import java.util.Arrays;
import java.util.EnumSet;
import java.util.List;
import javax.annotation.Nullable;

/** Static methods for converting back and forth from {@link Card.Rank} to {@code char}. */
public final class Ranks {
  private Ranks() {}

  public static final List<Card.Rank> HIGH_TO_LOW =
      ImmutableList.copyOf(Lists.reverse(Arrays.asList(Card.Rank.values())));

  public static final Converter<Character, Card.Rank> CHAR_TO_RANK =
      Converter.from(Ranks::char2Rank, Ranks::rank2Char);

  @Nullable
  public static Card.Rank char2Rank(char r) {
    return biData.get(Character.toUpperCase(r));
  }

  public static char rank2Char(Card.Rank r) {
    return biData.inverse().get(r);
  }

  // includes 9 - may be wrong
  public static EnumSet<Card.Rank> lowCards() {
    return LOW_CARDS;
  }

  private static ImmutableBiMap<Character, Card.Rank> biData =
      ImmutableBiMap.<Character, Card.Rank>builder()
          .put('2', Card.Rank.TWO)
          .put('3', Card.Rank.THREE)
          .put('4', Card.Rank.FOUR)
          .put('5', Card.Rank.FIVE)
          .put('6', Card.Rank.SIX)
          .put('7', Card.Rank.SEVEN)
          .put('8', Card.Rank.EIGHT)
          .put('9', Card.Rank.NINE)
          .put('T', Card.Rank.TEN)
          .put('J', Card.Rank.JACK)
          .put('Q', Card.Rank.QUEEN)
          .put('K', Card.Rank.KING)
          .put('A', Card.Rank.ACE)
          .build();

  private static final EnumSet<Card.Rank> LOW_CARDS =
      EnumSet.of(
          Card.Rank.TWO,
          Card.Rank.THREE,
          Card.Rank.FOUR,
          Card.Rank.FIVE,
          Card.Rank.SIX,
          Card.Rank.SEVEN,
          Card.Rank.EIGHT,
          Card.Rank.NINE);
}
