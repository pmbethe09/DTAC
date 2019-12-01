package edu.nyu.cards;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.cards.gen.Cards.Card.Rank;
import org.junit.Test;

public class RanksTest {
  @Test
  public void testRanks() {
    assertThat(Rank.KING).isEqualTo(Ranks.char2Rank('K'));
    assertThat('7').isEqualTo(Ranks.rank2Char(Ranks.char2Rank('7')));
  }
}
