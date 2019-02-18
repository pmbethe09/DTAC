package edu.nyu.bridge.util;

import static com.google.common.base.Functions.toStringFunction;

import com.google.common.collect.ImmutableList;
import com.google.common.collect.Iterables;
import edu.nyu.bridge.gen.Bridge.Deal;
import edu.nyu.bridge.gen.Bridge.DealOrBuilder;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.gen.Bridge.Vulnerability;
import edu.nyu.cards.Hand;
import java.util.List;
import java.util.Map;
import javax.annotation.Nullable;

/** Utilities for talking about Deal properties. */
public final class Deals {
  private Deals() {}

  private static final ImmutableList<Vulnerability> VUL_ROTATION =
      ImmutableList.copyOf(
          new Vulnerability[] {
            Vulnerability.NONE,
            Vulnerability.NS,
            Vulnerability.EW,
            Vulnerability.BOTH,
            Vulnerability.NS,
            Vulnerability.EW,
            Vulnerability.BOTH,
            Vulnerability.NONE,
            Vulnerability.EW,
            Vulnerability.BOTH,
            Vulnerability.NONE,
            Vulnerability.NS,
            Vulnerability.BOTH,
            Vulnerability.NONE,
            Vulnerability.NS,
            Vulnerability.EW
          });

  private static final ImmutableList<Direction> DEALERS =
      ImmutableList.of(Direction.WEST, Direction.NORTH, Direction.EAST, Direction.SOUTH);

  // Uses either vul field or board number index.
  // returns null if not available.
  @Nullable
  public static Vulnerability getVul(DealOrBuilder deal) {
    if (deal.hasVulnerable()) {
      return deal.getVulnerable();
    }
    if (!deal.hasBoard()) {
      return null;
    }
    return getVul(deal.getBoard());
  }

  public static Vulnerability getVul(int boardNumber) {
    return VUL_ROTATION.get((boardNumber - 1) % 16);
  }

  public static Direction getDealer(int boardNum) {
    return DEALERS.get(boardNum % 4);
  }

  public static Deal of(int boardNumber, Iterable<Hand> hands) {
    return Deal.newBuilder()
        .setBoard(boardNumber)
        .setDealer(getDealer(boardNumber))
        .setVulnerable(getVul(boardNumber))
        .addAllHands(Iterables.transform(hands, toStringFunction()))
        .build();
  }

  public static List<Hand> fromNorth(Map<Direction, Hand> deal) {
    return ImmutableList.of(
        deal.get(Direction.NORTH),
        deal.get(Direction.EAST),
        deal.get(Direction.SOUTH),
        deal.get(Direction.WEST));
  }
}
