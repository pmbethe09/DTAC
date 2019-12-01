package edu.nyu.cards;

import static com.google.common.truth.Truth.assertThat;

import org.junit.Test;

public class GenericHandTest {
  @Test
  public void testBasic() {
    GenericHand genericHand = GenericHand.fromString("AKxxx.KQx.J9.QTx");
    Hand hand = genericHand.toHand(genericHand.complement());
    assertThat(13).isEqualTo(hand.size());
  }
}
