package edu.nyu.cards;

import java.util.List;

import edu.nyu.cards.gen.Cards;

/** Describes an interface for generating {@link Deal}s. */
public interface Dealer {
  /**
   * Generate number of hands of cards.
   * Can also be used with hands=1 to generate a random deck ordering.
   */
  List<Cards.Hand> generateDeal(int hands, Cards.Hand... previous);

  /**
   * Returns {@code true} if more deals are available, which is usually true except if say
   * a tournament dealer, which uses random or prebuilt deals of a specific quantity.
   */
  boolean hasNext();
}
