package edu.nyu.cards.dealer;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.cards.Hand;
import edu.nyu.cards.gen.Cards;
import java.util.List;
import java.util.Random;
import org.junit.Test;

public class RandomDealerTest {
  @Test
  public void testDealing() {
    Hand hand = new Hand();
    for (Cards.Hand proto : new RandomDealer(new Random()).next()) {
      hand.addAll(proto);
    }
    assertThat(52).isEqualTo(hand.size());
  }

  @Test
  public void testDealHands() {
    Dealer dealer = new RandomDealer(new Random());
    List<Cards.Hand> deal = dealer.next();
    assertThat(4).isEqualTo(deal.size());
    assertThat(2).isEqualTo(dealer.generateDeal(2).size());
  }

  @Test
  public void testRepeatDealing() {
    for (int i = 0; i < 10; i++) {
      assertThat(new RandomDealer(new Random(i)).next())
          .isEqualTo(new RandomDealer(new Random(i)).next());
    }
  }

  @Test
  public void testRandomDealing() {
    for (int i = 0; i < 10; i++) {
      assertThat(new RandomDealer(new Random(i)).next())
          .isNotEqualTo(new RandomDealer(new Random(i + 1)).next());
    }
  }
}
