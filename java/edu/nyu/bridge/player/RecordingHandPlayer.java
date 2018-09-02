package edu.nyu.bridge.player;

import edu.nyu.bridge.gen.Bridge.Direction;

/** Extends HandPlayer providing information about the state of this hand. */
public interface RecordingHandPlayer extends HandPlayer {
  /** Returns if this player thinks it is its turn to bid. */
  boolean isMyBid();

  /** Returns which player this hand thinks it is. */
  Direction currentBidder();

  /** Returns true if the auction is still happening. */
  boolean inAuction();

  /** Returns true from after bidding complete until last trick or claim. */
  boolean inPlay();

  /** Returns true if it is this hand's turn to play. */
  boolean isMyPlay();

  /** Returns the number of the trick being played. 0 means none yet, 14 means hand over. */
  int currentTrick();
}
