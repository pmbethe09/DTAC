package edu.nyu.cards.dealer;

import static com.google.common.base.Preconditions.checkNotNull;
import static java.util.stream.Collectors.toList;

import com.google.common.collect.Lists;
import edu.nyu.cards.Hand;
import edu.nyu.cards.Hands;
import edu.nyu.cards.gen.Cards;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import javax.inject.Inject;

/**
 * Implementation of {@link Dealer} that uses the provided source of randomness to shuffle the
 * available cards and then partition into the requested number of hands.
 */
public class RandomDealer implements Dealer {
  private final Random random;

  @Inject
  public RandomDealer(Random random) {
    this.random = checkNotNull(random, "random");
  }

  @Override
  public List<Cards.Hand> generateDeal(int hands, Cards.Hand... excluded) {
    Hand deck = Hand.deck();
    for (Cards.Hand proto : excluded) {
      deck.removeAll(Hand.fromProto(proto));
    }
    List<Cards.Card> result = Lists.newArrayList(deck.toProto().getCardsList());
    Collections.shuffle(result, random);
    return Lists.partition(result, result.size() / hands)
        .stream()
        .map(Hands.HAND_TO_LIST.reverse())
        .collect(toList());
  }

  @Override
  public boolean hasNext() {
    return true;
  }

  @Override
  public List<Cards.Hand> next() {
    return generateDeal(4);
  }
}
