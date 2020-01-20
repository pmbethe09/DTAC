package edu.nyu.cards;

import static com.google.common.truth.Truth.assertThat;

import com.google.common.collect.ImmutableList;
import edu.nyu.cards.gen.Cards.Card;
import edu.nyu.cards.gen.Cards.Card.Rank;
import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class CardsTest {
  @Test
  public void testConvert() {
    assertThat("H2").isEqualTo(Cards.card2String(Cards.string2Card("H2")));
  }

  @Test
  public void testEnum() {
    assertThat(Card.newBuilder().setRank(Rank.TWO).setSuit(Suit.HEARTS).build())
        .isEqualTo(Cards.string2Card("H2"));
  }

  @Test
  public void testCompare() {
    ImmutableList<String> asc =
        ImmutableList.of("SA", "S6", "HK", "H4", "DQ", "DJ", "D2", "C7", "C2");
    for (int i = 0; i < asc.size() - 1; i++) {
      Card icard = Cards.string2Card(asc.get(i));
      assertThat(Cards.compare(icard, icard)).isEqualTo(0);
      for (int j = i + 1; j < asc.size(); j++) {
        Card jcard = Cards.string2Card(asc.get(j));
        assertThat(Cards.compare(icard, jcard)).isGreaterThan(0);
        assertThat(Cards.compare(jcard, icard)).isLessThan(0);
      }
    }
  }
}
