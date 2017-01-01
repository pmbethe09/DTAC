package edu.nyu.cards;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import edu.nyu.cards.gen.Cards.Card.Rank;

public class RanksTest {
  @Test
  public void testRanks() {
    assertEquals("2convertRank", Rank.KING, Ranks.char2Rank('K'));
    assertEquals("Seven", '7', Ranks.rank2Char(Ranks.char2Rank('7')));
  }
}
