package edu.nyu.bridge.util;

import static com.google.common.collect.ImmutableList.toImmutableList;
import static com.google.common.truth.Truth.assertThat;
import static edu.nyu.cards.Cards.string2Card;

import com.google.common.collect.ImmutableList;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.Cards;
import edu.nyu.cards.Hand;
import edu.nyu.cards.gen.Cards.Card;
import edu.nyu.cards.gen.Cards.Suit;
import java.util.List;
import org.junit.Test;

public class TricksTest {
  @Test
  public void testHigh() {
    Card s5 = string2Card("s5");
    Card sK = string2Card("SK");
    Card hJ = string2Card("hj");
    assertThat(sK).isEqualTo(Tricks.high(s5, sK, null));
    assertThat(s5).isEqualTo(Tricks.high(s5, hJ, null));
    assertThat(hJ).isEqualTo(Tricks.high(s5, hJ, Suit.HEARTS));
    assertThat(s5).isEqualTo(Tricks.high(s5, hJ, Suit.CLUBS));
  }

  @Test
  public void testLegal() {
    Hand hand = Hand.fromString("AKQ.KQJ..9753");
    assertThat(Tricks.legal(string2Card("SK"), hand, Suit.SPADES)).isTrue();
    assertThat(Tricks.legal(string2Card("SK"), hand, Suit.DIAMONDS)).isTrue();
    assertThat(Tricks.legal(string2Card("SK"), hand, Suit.HEARTS)).isFalse();
    assertThat(Tricks.legal(string2Card("H2"), hand, Suit.HEARTS)).isFalse();
  }

  @Test
  public void testWinner() {
    assertThat(Direction.NORTH)
        .isEqualTo(Tricks.winner(trick("SK", "S5", "ST", "S9"), Direction.NORTH, null));
    assertThat(Direction.NORTH)
        .isEqualTo(Tricks.winner(trick("SK", "S5", "SA", "S9"), Direction.SOUTH, null));
    assertThat(Direction.NORTH)
        .isEqualTo(Tricks.winner(trick("SK", "S5", "SA", "D9"), Direction.SOUTH, null));
    assertThat(Direction.EAST)
        .isEqualTo(Tricks.winner(trick("SK", "S5", "SA", "D9"), Direction.SOUTH, Suit.DIAMONDS));
    assertThat(Direction.WEST)
        .isEqualTo(Tricks.winner(trick("SA", "D9"), Direction.SOUTH, Suit.DIAMONDS));
    assertThat(Direction.NORTH)
        .isEqualTo(Tricks.winner(trick("SA", "D9", "DJ"), Direction.SOUTH, Suit.DIAMONDS));
    assertThat(Direction.WEST)
        .isEqualTo(Tricks.winner(trick("SA", "D9", "D8"), Direction.SOUTH, Suit.DIAMONDS));
  }

  private static List<Card> trick(String... cards) {
    return ImmutableList.copyOf(cards).stream().map(Cards::string2Card).collect(toImmutableList());
  }
}
