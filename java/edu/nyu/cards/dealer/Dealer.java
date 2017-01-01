package edu.nyu.cards.dealer;

import java.util.Iterator;
import java.util.List;

import edu.nyu.cards.gen.Cards;

/** Describes an interface for generating {@link Deal}s.
 *
 * Implements {@link Iterator} for convenience.
 *
 * The inherited {@code hasNext} method returns {@code true} if more deals are available,
 * which is usually true except if say a tournament dealer,
 * which uses random or prebuilt deals of a specific quantity.
 *
 * The inherited {@code next} method is a short cut for {@code generateDeal(4)} assuming a 4-player
 * card game.
 */
public interface Dealer extends Iterator<List<Cards.Hand>> {
  /**
   * Generate the given number of hands of cards.
   *
   * @param hands the number of partitions to make.
   * @param excluded cards to exclude from being dealt out, e.g. to examine an end position where
   *        some cards have already been played.
   * @return a {@link List} of {@link Cards.Hand} containing the remaining cards split evenly as per
   *         the algorithm in play.
   */
  List<Cards.Hand> generateDeal(int hands, Cards.Hand... excluded);
}
