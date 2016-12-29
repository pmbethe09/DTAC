package edu.nyu.cards;

import static com.google.common.base.Preconditions.checkNotNull;

import java.util.EnumMap;
import java.util.Map;

import com.google.common.collect.ImmutableMap;
import com.google.common.collect.Maps;

import edu.nyu.cards.gen.Cards.Card;
import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;

/**
 * Helper function for stringifying and extracting constants for {@link Card}s
 */
public class Cards {
  private static final Map<Suit, Map<Rank, String>> CARD_2_STRING;
  private static final Map<Suit, Map<Rank, Card>> PREBUILT_CARDS;
  private static final Map<String, Card> STRING_2_CARD;

  static {
    ImmutableMap.Builder<String, Card> builder = ImmutableMap.builder();
    ImmutableMap.Builder<Suit, Map<Rank, String>> cardBuilder = ImmutableMap.builder();
    ImmutableMap.Builder<Suit, Map<Rank, Card>> preBuilder = ImmutableMap.builder();
    for (Suit suit : Suit.values()) {
      EnumMap<Rank, Card> thisSuit = Maps.newEnumMap(Rank.class);
      EnumMap<Rank, String> thisSuitStrings = Maps.newEnumMap(Rank.class);
      for (Rank rank : Rank.values()) {
        Card card = Card.newBuilder().setSuit(suit).setRank(rank).build();
        builder.put(Suits.suit2Char(suit) + "" + Ranks.rank2Char(rank), card);
        thisSuit.put(rank, card);
        thisSuitStrings.put(rank, internalCard2String(suit, rank));
      }
      preBuilder.put(suit, Maps.immutableEnumMap(thisSuit));
      cardBuilder.put(suit, Maps.immutableEnumMap(thisSuitStrings));
    }
    builder.put("-", Card.getDefaultInstance());
    STRING_2_CARD = builder.build();
    CARD_2_STRING = Maps.immutableEnumMap(cardBuilder.build());
    PREBUILT_CARDS = Maps.immutableEnumMap(preBuilder.build());
  }

  private static String internalCard2String(Suit suit, Rank rank) {
    return new StringBuffer()
        .append(Suits.suit2Char(suit))
        .append(Ranks.rank2Char(rank))
        .toString();
  }

  /** Returns a prebuilt {@link Card} for the given string. */
  public static Card string2Card(String cardString) {
    if (cardString.equals("") || cardString.equals("-")) {
      return Card.getDefaultInstance();
    }
    return checkNotNull(
        STRING_2_CARD.get(cardString.toUpperCase()), "no card found for %s", cardString);
  }

  /** Returns the standard SuitRank e.g. DA */
  public static String card2String(Card card) {
    if (!card.hasRank() && !card.hasSuit()) {
      return "-";
    }
    return card2String(card.getSuit(), card.getRank());
  }

  /** Returns the standard SuitRank e.g. DA */
  public static String card2String(Suit suit, Rank rank) {
    return CARD_2_STRING.get(suit).get(rank);
  }

  /** Returns a prebuilt {@link Card} for the given {@link Suit}, {@link Rank}. */
  public static Card card(Suit suit, Rank rank) {
    return PREBUILT_CARDS.get(suit).get(rank);
  }
}
