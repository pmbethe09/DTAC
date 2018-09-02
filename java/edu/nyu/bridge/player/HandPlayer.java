package edu.nyu.bridge.player;

import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.scoring.Result;
import edu.nyu.cards.gen.Cards.Card;
import edu.nyu.cards.gen.Cards.Hand;
import java.io.Closeable;
import java.util.List;
import javax.annotation.concurrent.NotThreadSafe;

/**
 * Knows about current hand, and tracks state. Only good for the current hand, not expected to be
 * thread-safe if multiple calls happen simultaneously.
 */
@NotThreadSafe
public interface HandPlayer extends Closeable {
  /** Called when it is this hand's turn to bid. */
  Call makeBid();

  /** The currentBidder (not us) made this bid. */
  void nextBid(Call waitForBid);

  /** Play to the current trick by returning a {@link Card} from our hand. */
  Card makePlay(List<Card> current);

  /** Called when this hand is declarer and needs to play from dummy to the current trick */
  Card playDummy(List<Card> current);

  /** The next player to play (not us), has played this card. */
  void nextPlay(Card card);

  /** Called after the opening lead to get dummy's cards. */
  void dummy(Hand hand);

  /**
   * The current player to play has claimed this many declarer tricks (with description for human
   * players.
   */
  boolean doesAcceptClaim(Hand claimer, Result claimed, String description);
}
