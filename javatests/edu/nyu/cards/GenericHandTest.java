package edu.nyu.cards;

import org.junit.Test;

import junit.framework.TestCase;

public class GenericHandTest extends TestCase {
  @Test
  public void testBasic() {
    GenericHand genericHand = GenericHand.fromString("AKxxx.KQx.J9.QTx");
    Hand hand = genericHand.toHand(genericHand.complement());
    assertEquals("13 cards", 13, hand.size());
  }
}
