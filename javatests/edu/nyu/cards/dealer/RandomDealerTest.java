package edu.nyu.cards.dealer;

import java.util.Random;

import org.junit.Test;

import edu.nyu.cards.Hand;
import edu.nyu.cards.gen.Cards;
import junit.framework.TestCase;

public class RandomDealerTest extends TestCase {
  @Test
  public void testDealing() {
    Hand hand = new Hand();
    for (Cards.Hand proto : new RandomDealer(new Random()).next()) {
      hand.addAll(proto);
    }
    assertEquals("has all", 52, hand.size());
  }

  @Test
  public void testRepeatDealing() {
    for (int i = 0; i < 10; i++) {
      assertEquals("same seed deal " + i, new RandomDealer(new Random(i)).next(),
          new RandomDealer(new Random(i)).next());
    }
  }

  @Test
  public void testRandomDealing() {
    for (int i = 0; i < 10; i++) {
      assertNotSame("different seed deal " + i, new RandomDealer(new Random(i)).next(),
          new RandomDealer(new Random(i + 1)).next());
    }
  }
}
