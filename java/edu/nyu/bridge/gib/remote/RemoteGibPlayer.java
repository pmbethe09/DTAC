package edu.nyu.bridge.gib.remote;

import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.player.BridgePlayer;
import edu.nyu.bridge.player.HandPlayer;
import edu.nyu.cards.gen.Cards;

/**
 * Implements the {@link BridgePlayer}/{@link HandPlayer} protocols
 * while using an underlying commandline GiB session to determine moves.
 */
public class RemoteGibPlayer implements BridgePlayer {
  private static final String GIBREST_URL = "https://gibrest.bridgebase.com/u_bm/robot.php";

  @Override
  public HandPlayer newDeal(Cards.Hand hand, int boardNumber, Direction direction) {
    return null;
  }
}