package edu.nyu.bridge.util;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.gen.Cards.Suit;

public class ContractTest {
  @Test
  public void testPbnParse() {
    assertEquals(
        "same both ways", Contract.of(8, Suit.CLUBS, Direction.SOUTH), Contract.parse("2CS"));
    assertEquals(
        "same both ways", Contract.of(11, Suit.DIAMONDS, Direction.EAST), Contract.parse("5DE"));
  }

  @Test
  public void testOf() {
    assertEquals("deal with tricks/BOOK", Contract.of(10, Suit.SPADES, Direction.WEST),
        Contract.of(Call.newBuilder().setBid(Bid.FOUR_SPADES).build(), Direction.WEST));
  }

  @Test
  public void testBeats() {
    assertTrue(contract("3C").beats(contract("2N")));
    assertTrue(contract("3S").beats(contract("1N")));
    assertFalse(contract("3C").beats(contract("3H")));
    assertFalse(contract("1N").beats(contract("2D")));
  }

  @Test
  public void testBonus() {
    assertEquals(Contract.Bonus.GAME, contract("3N").bonus());
    assertEquals(Contract.Bonus.GAME, contract("4N").bonus());
    assertEquals(Contract.Bonus.PARTSCORE, contract("3S").bonus());
    assertEquals(Contract.Bonus.GAME, contract("4S").bonus());
    assertEquals(Contract.Bonus.PARTSCORE, contract("4D").bonus());
    assertEquals(Contract.Bonus.GAME, contract("5D").bonus());
    assertEquals(Contract.Bonus.SLAM, contract("6D").bonus());
    assertEquals(Contract.Bonus.GRAND, contract("7C").bonus());
  }

  private static Contract contract(String c) {
    return Contract.of(Calls.string2Call(c), Direction.WEST);
  }
}
