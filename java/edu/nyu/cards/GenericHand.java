package edu.nyu.cards;

import static edu.nyu.cards.Suits.iterateSuitsHighLow;
import static edu.nyu.cards.Suits.lowerSuit;

import com.google.common.base.Preconditions;
import com.google.common.collect.ImmutableMap;
import com.google.common.collect.Maps;
import edu.nyu.cards.gen.Cards.Card;
import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;
import java.util.EnumMap;
import java.util.EnumSet;
import java.util.Map;

/**
 * A Generic hand is used for hands with unspecified low cards.
 *
 * <p>E.g. "AK 5th, 3 small, A and 1, J third", which is written "AKxxx.xxx.Ax.Jxx" Where each x
 * represents any low card, usually below 9 or T.
 */
public class GenericHand {
  /** Returns a GenericHand parsed from the normal string format but with 'x's. */
  public static GenericHand fromString(String hand) {
    GenericHand.Builder result = new Builder();
    // TODO(pbethe): support several formats...
    Suit currentSuit = Suit.NOTRUMPS;
    Rank lastRank = Rank.TWO;
    for (char c : hand.toCharArray()) {
      Rank rank = Ranks.char2Rank(c);
      boolean isGeneric = c == 'x' || c == 'X';
      if (rank == null && !isGeneric) {
        // treat as break - prob '.' or ' '
        // TODO multiple spaces as a single suit change
        currentSuit = lowerSuit(currentSuit);
        lastRank = null;
        continue;
      }
      if (!isGeneric && lastRank != null && rank.getNumber() >= lastRank.getNumber()) {
        // new suit
        currentSuit = lowerSuit(currentSuit);
      }
      if (isGeneric) {
        if (currentSuit == Suit.NOTRUMPS) {
          currentSuit = Suit.SPADES;
        }
        result.addLowCard(currentSuit);
      } else {
        result.addCard(currentSuit, rank);
      }
      lastRank = rank;

      // lowerSuit
    }
    return result.build();
  }

  private final ImmutableMap<Suit, Integer> lowCards;
  private final Hand knownCards;

  private GenericHand(Map<Suit, Integer> lowCards, Hand knownCards) {
    this.lowCards = Maps.immutableEnumMap(lowCards);
    this.knownCards = knownCards;
  }

  /** Returns a {@link Hand} with all cards not explicitly in this hand. */
  public Hand complement() {
    Hand result = new Hand();
    for (Suit suit : iterateSuitsHighLow()) {
      result.addCards(suit, EnumSet.complementOf(knownCards.suit(suit)));
    }
    return result;
  }

  /**
   * Returns a real {@link Hand} from this one, using the provided available cards to replace
   * generic 'x' with actual cards.
   */
  public Hand toHand(Hand availableCards) {
    Hand result = new Hand();
    for (Suit suit : iterateSuitsHighLow()) {
      result.addCards(suit, knownCards.suit(suit));
      int lowCardsNeeded = lowCards.get(suit);
      for (Rank rank : Ranks.lowCards()) {
        if (lowCardsNeeded == 0) {
          break;
        }
        if (availableCards.hasCard(suit, rank)) {
          result.addCard(suit, rank);
          availableCards.removeCard(suit, rank);
          --lowCardsNeeded;
        }
      }
      Preconditions.checkArgument(
          lowCardsNeeded == 0,
          "Unable to find %s %s low cards from %s",
          lowCardsNeeded,
          suit,
          availableCards);
    }
    return result;
  }

  @Override
  public String toString() {
    // make same string as parseable in fromString
    StringBuffer result = new StringBuffer();
    for (Suit suit : iterateSuitsHighLow()) {
      for (Card.Rank rank : Ranks.HIGH_TO_LOW) {
        if (knownCards.suit(suit).contains(rank)) {
          result.append(Ranks.rank2Char(rank));
        }
      }
      for (int i = 0; i < lowCards.get(suit); ++i) {
        result.append('x');
      }
      if (suit != Suit.CLUBS) {
        result.append('.');
      }
    }
    return result.toString();
  }

  @Override
  public int hashCode() {
    return toString().hashCode();
  }

  @Override
  public boolean equals(Object other) {
    if (!(other instanceof GenericHand)) {
      return false;
    }
    GenericHand otherHand = (GenericHand) other;
    return knownCards.equals(otherHand.knownCards) && lowCards.equals(otherHand.lowCards);
  }

  public static class Builder {
    private final EnumMap<Suit, Integer> lowCards;
    private final Hand knownCards = new Hand();

    public Builder() {
      lowCards = Maps.newEnumMap(Suit.class);
      for (Suit suit : Suits.iterateSuitsHighLow()) {
        lowCards.put(suit, 0);
      }
    }

    public void addCard(Suit suit, Rank rank) {
      knownCards.addCard(suit, rank);
    }

    public void addLowCard(Suit suit) {
      lowCards.put(suit, 1 + lowCards.get(suit));
    }

    public GenericHand build() {
      return new GenericHand(lowCards, knownCards);
    }
  }
}
