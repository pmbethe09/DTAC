package edu.nyu.bridge.player;

import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.cards.gen.Cards.Hand;
import javax.annotation.concurrent.ThreadSafe;

/**
 * Interface for a persistent bridge player that may learn on the go and be used to play many hands.
 */
@ThreadSafe
public interface BridgePlayer {
  /** Given a hand, etc, construct a HandPlayer to track and play this hand. */
  HandPlayer newDeal(Hand hand, int boardNumber, Direction direction);
}
