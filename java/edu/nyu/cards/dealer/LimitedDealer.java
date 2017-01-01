package edu.nyu.cards.dealer;

import java.util.List;

import edu.nyu.cards.gen.Cards.Hand;

/** Deals only N deals from the underlying dealer. */
public class LimitedDealer implements Dealer {
  // TODO boards and dealing args
  private final Dealer dealer;
  private int nDeals;

  public LimitedDealer(Dealer dealer, int nDeals) {
    this.dealer = dealer;
    this.nDeals = nDeals;
  }

  public static LimitedDealer of(Dealer dealer, int nDeals) {
    return new LimitedDealer(dealer, nDeals);
  }

  @Override
  public List<Hand> generateDeal(int hands, Hand... previous) {
    --nDeals;
    return dealer.generateDeal(hands, previous);
  }

  @Override
  public boolean hasNext() {
    return nDeals > 0 && dealer.hasNext();
  }

  @Override
  public List<Hand> next() {
    return generateDeal(4);
  }
}
