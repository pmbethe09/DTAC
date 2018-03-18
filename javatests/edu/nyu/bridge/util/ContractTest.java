package edu.nyu.bridge.util;

import static com.google.common.truth.Truth.assertThat;

import org.junit.Test;

import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.gen.Cards.Suit;

public class ContractTest {
  @Test
  public void testPbnParse() {
    assertThat(Contract.of(8, Suit.CLUBS, Direction.SOUTH))
        .named("same both ways")
        .isEqualTo(Contract.parse("2CS"));
    assertThat(Contract.of(11, Suit.DIAMONDS, Direction.EAST))
        .named("same both ways")
        .isEqualTo(Contract.parse("5DE"));
  }

  @Test
  public void testOf() {
    assertThat(Contract.of(10, Suit.SPADES, Direction.WEST))
        .named("deal with tricks/BOOK")
        .isEqualTo(Contract.of(Call.newBuilder().setBid(Bid.FOUR_SPADES).build(), Direction.WEST));
  }

  @Test
  public void testBeats() {
    assertThat(contract("3C").beats(contract("2N"))).isTrue();
    assertThat(contract("3S").beats(contract("1N"))).isTrue();
    assertThat(contract("3C").beats(contract("3H"))).isFalse();
    assertThat(contract("1N").beats(contract("2D"))).isFalse();
  }

  @Test
  public void testBonus() {
    assertThat(Contract.Bonus.GAME).isEqualTo(contract("3N").bonus());
    assertThat(Contract.Bonus.GAME).isEqualTo(contract("4N").bonus());
    assertThat(Contract.Bonus.PARTSCORE).isEqualTo(contract("3S").bonus());
    assertThat(Contract.Bonus.GAME).isEqualTo(contract("4S").bonus());
    assertThat(Contract.Bonus.PARTSCORE).isEqualTo(contract("4D").bonus());
    assertThat(Contract.Bonus.GAME).isEqualTo(contract("5D").bonus());
    assertThat(Contract.Bonus.SLAM).isEqualTo(contract("6D").bonus());
    assertThat(Contract.Bonus.GRAND).isEqualTo(contract("7C").bonus());
  }

  private static Contract contract(String c) {
    return Contract.of(Calls.string2Call(c), Direction.WEST);
  }
}
