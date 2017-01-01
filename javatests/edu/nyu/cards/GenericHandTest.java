package edu.nyu.cards;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class GenericHandTest {
  @Test
  public void testBasic() {
    GenericHand genericHand = GenericHand.fromString("AKxxx.KQx.J9.QTx");
    Hand hand = genericHand.toHand(genericHand.complement());
    assertEquals("13 cards", 13, hand.size());
  }
}
