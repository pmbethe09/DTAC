package edu.nyu.cards.dealer;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import com.google.common.base.Preconditions;
import edu.nyu.cards.gen.Cards;

/** Implements a {@link Dealer} using a pre-canned collection of {@link Deal}s. */
public class PresetDealer implements Dealer {
  private final Iterator<Cards.Deal> deals;

  public PresetDealer(Collection<Cards.Deal> deals) {
    this.deals = deals.iterator();
  }

  public static PresetDealer of(Collection<Cards.Deal> deals) {
    return new PresetDealer(deals);
  }

  @Override
  public List<Cards.Hand> generateDeal(int hands, Cards.Hand... previous) {
    Preconditions.checkArgument(previous.length == 0, "Preset dealer can't handle previous hands.");
    List<Cards.Hand> result = deals.next().getHandsList();

    Preconditions.checkArgument(
        hands == result.size(), "mismatch, preset size=%s, requested=%s", result.size(), hands);
    return result;
  }

  @Override
  public boolean hasNext() {
    return deals.hasNext();
  }

  @Override
  public List<Cards.Hand> next() {
    return deals.next().getHandsList();
  }
}
