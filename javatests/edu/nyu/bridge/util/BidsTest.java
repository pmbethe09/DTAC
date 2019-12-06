package edu.nyu.bridge.util;

import static com.google.common.truth.Truth.assertThat;

import com.google.common.collect.Sets;
import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Level;
import edu.nyu.cards.gen.Cards.Suit;
import org.junit.Test;

public class BidsTest {
  @Test
  public void testThereAndBackAgain() {
    assertThat(Bids.bid(Level.TWO, Suit.NOTRUMPS)).isEqualTo(Bid.TWO_NOTRUMPS);
    assertThat(Bids.bid(Level.THREE, Suit.SPADES)).isEqualTo(Bid.THREE_SPADES);
    assertThat(Bids.bid(Level.FOUR, Suit.HEARTS)).isEqualTo(Bid.FOUR_HEARTS);
    assertThat(Bids.bid(Level.SIX, Suit.CLUBS)).isEqualTo(Bid.SIX_CLUBS);
  }

  @Test
  public void testBidRange() {
    assertThat(Sets.complementOf(Bids.bidRangeAfter(Bid.ONE_HEART)))
        .containsExactly(Bid.ONE_HEART, Bid.ONE_DIAMOND, Bid.ONE_CLUB);
  }
}
