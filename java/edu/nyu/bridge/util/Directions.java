package edu.nyu.bridge.util;

import static edu.nyu.bridge.gen.Bridge.Direction.EAST;
import static edu.nyu.bridge.gen.Bridge.Direction.NORTH;
import static edu.nyu.bridge.gen.Bridge.Direction.SOUTH;
import static edu.nyu.bridge.gen.Bridge.Direction.WEST;

import com.google.common.base.Function;
import com.google.common.collect.ImmutableList;
import com.google.common.collect.ImmutableMap;
import com.google.common.collect.ImmutableSet;
import com.google.common.collect.Maps;
import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.gen.Bridge.Direction;
import java.util.EnumMap;
import java.util.List;
import javax.annotation.Nullable;

/** Utility methods for getting info related to {@link Direction}. */
public final class Directions {
  private Directions() {}

  private static final ImmutableMap<Direction, ImmutableList<Direction>> DIRECTION_PLUS_FOUR =
      Maps.immutableEnumMap(
          ImmutableMap.of(
              NORTH,
              ImmutableList.of(NORTH, EAST, SOUTH, WEST),
              EAST,
              ImmutableList.of(EAST, SOUTH, WEST, NORTH),
              SOUTH,
              ImmutableList.of(SOUTH, WEST, NORTH, EAST),
              WEST,
              ImmutableList.of(WEST, NORTH, EAST, SOUTH)));

  private static final ImmutableMap<Direction, ImmutableList<Direction>> OTHERS =
      Maps.immutableEnumMap(
          ImmutableMap.of(
              NORTH,
              ImmutableList.of(EAST, SOUTH, WEST),
              EAST,
              ImmutableList.of(SOUTH, WEST, NORTH),
              SOUTH,
              ImmutableList.of(WEST, NORTH, EAST),
              WEST,
              ImmutableList.of(NORTH, EAST, SOUTH)));

  public static <T> ImmutableMap<Direction, T> map(Function<Direction, T> func) {
    return Maps.<Direction, T>immutableEnumMap(
        Maps.<Direction, T>asMap(ImmutableSet.copyOf(Bridge.Direction.values()), func));
  }

  public static ImmutableList<Direction> iterateDirections(Direction start) {
    return DIRECTION_PLUS_FOUR.get(start);
  }

  public static ImmutableList<Direction> others(Direction start) {
    return OTHERS.get(start);
  }

  @Nullable
  public static Direction fromString(@Nullable String input) {
    if (input == null || input.length() == 0) {
      return null;
    }
    switch (input.charAt(0)) {
      case 'n':
      case 'N':
        return NORTH;
      case 'e':
      case 'E':
        return EAST;
      case 's':
      case 'S':
        return SOUTH;
      case 'w':
      case 'W':
        return WEST;
      default:
        throw new IllegalArgumentException("Expected a string started with n/e/s/w");
    }
  }

  public static Direction lho(Direction current) {
    switch (current) {
      case NORTH:
        return EAST;
      case EAST:
        return SOUTH;
      case SOUTH:
        return WEST;
      case WEST:
        return NORTH;
      default:
        throw new UnsupportedOperationException("Unexpected direction: " + current);
    }
  }

  public static Direction rho(Direction current) {
    switch (current) {
      case NORTH:
        return WEST;
      case EAST:
        return NORTH;
      case SOUTH:
        return EAST;
      case WEST:
        return SOUTH;
      default:
        throw new UnsupportedOperationException("Unexpected direction: " + current);
    }
  }

  public static Direction partner(Direction current) {
    switch (current) {
      case NORTH:
        return SOUTH;
      case EAST:
        return WEST;
      case SOUTH:
        return NORTH;
      case WEST:
        return EAST;
      default:
        throw new UnsupportedOperationException("Unexpected direction: " + current);
    }
  }

  public static int offset(Direction dealer, Direction player) {
    if (dealer == player) {
      return 0;
    }
    if (lho(dealer) == player) {
      return 1;
    }
    if (partner(dealer) == player) {
      return 2;
    }
    if (rho(dealer) == player) {
      return 3;
    }
    throw new UnsupportedOperationException(
        "Unexpected direction: " + player + " is not lho/partner/rho of dealer: " + dealer);
  }

  public static boolean isNS(Direction player) {
    return player == Direction.NORTH || player == Direction.SOUTH;
  }

  public static boolean opponents(Direction player, Direction other) {
    return isNS(player) != isNS(other);
  }

  public static final EnumMap<Direction, String> HUMAN_STRINGS =
      Maps.newEnumMap(
          ImmutableMap.of(
              Direction.NORTH,
              "North",
              Direction.EAST,
              "East",
              Direction.SOUTH,
              "South",
              Direction.WEST,
              "West"));

  public static String human(Direction dir) {
    return HUMAN_STRINGS.get(dir);
  }

  public static Direction add(Direction dealer, int auctionCount) {
    switch (auctionCount & 3) { // mod 4
      case 0:
        return dealer;
      case 1:
        return lho(dealer);
      case 2:
        return partner(dealer);
      case 3:
        return rho(dealer);
      default:
        throw new IllegalStateException("can't get here mod 4");
    }
  }

  /**
   * Takes an array of hands oriented from {@code NORTH}, and returns in the specified direction.
   */
  public static <T> ImmutableList<T> fromNorth(Bridge.Direction dealer, T... hands) {
    return fromNorth(dealer, ImmutableList.copyOf(hands));
  }

  public static <T> ImmutableList<T> fromNorth(Bridge.Direction dealer, List<T> hands) {
    int offset;
    switch (dealer) {
      case NORTH:
        offset = 0;
        break;
      case EAST:
        offset = 1;
        break;
      case SOUTH:
        offset = 2;
        break;
      case WEST:
        offset = 3;
        break;
      default:
        throw new IllegalArgumentException("unexpected value of dealer");
    }
    ImmutableList.Builder<T> result = ImmutableList.builder();
    for (int i = offset; i < hands.size(); ++i) {
      result.add(hands.get(i));
    }
    for (int i = 0; i < offset; ++i) {
      result.add(hands.get(i));
    }
    return result.build();
  }

  /** Takes an array of hands oriented as {@code direction} and re-orients as {@code NORTH}. */
  public static <T> ImmutableList<T> toNorth(Bridge.Direction direction, T... hands) {
    return toNorth(direction, ImmutableList.copyOf(hands));
  }

  public static <T> ImmutableList<T> toNorth(Bridge.Direction direction, List<T> hands) {
    return isNS(direction) ? fromNorth(direction, hands) : fromNorth(partner(direction), hands);
  }
}
