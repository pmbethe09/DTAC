package edu.nyu.cards.dealer;

import static com.google.common.truth.Truth.assertThat;

import java.util.List;
import java.util.Random;

import org.junit.Test;

import edu.nyu.cards.Hand;
import edu.nyu.cards.gen.Cards;

public class RandomDealerTest {
  @Test
  public void testDealing() {
    Hand hand = new Hand();
    for (Cards.Hand proto : new RandomDealer(new Random()).next()) {
      hand.addAll(proto);
    }
    assertThat(52).named("has all").isEqualTo(hand.size());
  }

  @Test
  public void testDealHands() {
    Dealer dealer = new RandomDealer(new Random());
    List<Cards.Hand> deal = dealer.next();
    assertThat(4).named("has 4").isEqualTo(deal.size());
    assertThat(2).named("has 2").isEqualTo(dealer.generateDeal(2).size());
  }

  @Test
  public void testRepeatDealing() {
    for (int i = 0; i < 10; i++) {
      assertThat(new RandomDealer(new Random(i)).next())
          .named("same seed deal %s", i)
          .isEqualTo(new RandomDealer(new Random(i)).next());
    }
  }

  @Test
  public void testRandomDealing() {
    for (int i = 0; i < 10; i++) {
      assertThat(new RandomDealer(new Random(i)).next())
          .named("different seed deal %s", i)
          .isNotEqualTo(new RandomDealer(new Random(i + 1)).next());
    }
  }
}
