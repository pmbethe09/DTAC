package edu.nyu.bridge.util;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class ContractTest {
  @Test
  public void testDSL() {
    assertThat(Contract.of(8, Suit.CLUBS, Direction.SOUTH).doubled())
        .isEqualTo(Contract.parse("2CXS"));
    assertThat(Contract.of(11, Suit.DIAMONDS, Direction.EAST).redoubled())
        .isEqualTo(Contract.parse("5DXXE"));
  }

  @Test
  public void testAtLevel() {
    assertThat(Contract.of(8, Suit.CLUBS, Direction.SOUTH).doubled().atLevel(Bridge.Level.THREE))
            .isEqualTo(Contract.parse("3CS"));
    Contract c2 = Contract.of(8, Suit.CLUBS, Direction.SOUTH);
    assertThat(c2.atLevel(Bridge.Level.TWO))
            .isSameInstanceAs(c2);
  }

  @Test
  public void testPbnParse() {
    assertThat(Contract.of(8, Suit.CLUBS, Direction.SOUTH)).isEqualTo(Contract.parse("2CS"));
    assertThat(Contract.of(11, Suit.DIAMONDS, Direction.EAST)).isEqualTo(Contract.parse("5DE"));
  }

  @Test
  public void testOf() {
    assertThat(Contract.of(10, Suit.SPADES, Direction.WEST))
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
