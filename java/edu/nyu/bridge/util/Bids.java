package edu.nyu.bridge.util;

import static com.google.common.base.Preconditions.checkArgument;
import static com.google.common.collect.Maps.newEnumMap;

import com.google.common.base.Preconditions;
import com.google.common.collect.ImmutableList;
import com.google.common.collect.ImmutableMap;
import com.google.common.collect.Maps;
import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Level;
import edu.nyu.cards.gen.Cards.Suit;
import java.util.EnumMap;
import javax.annotation.Nullable;

/** Provides helper methods for switching between {@link Bid} and {@link Suit}/{@link Level}. */
public final class Bids {
  private Bids() {}

  private static final ImmutableMap<Level, EnumMap<Suit, Bid>> BID_MAP;
  private static final ImmutableMap<Bid, Level> BID_TO_LEVEL;
  private static final ImmutableMap<Bid, Suit> BID_TO_SUIT;

  public static Level level(Bid bid) {
    return BID_TO_LEVEL.get(bid);
  }

  public static Suit suit(Bid bid) {
    return BID_TO_SUIT.get(bid);
  }

  public static Bid bid(Level level, Suit suit) {
    return BID_MAP.get(level).get(suit);
  }

  /** Returns all bids (after, 7NT], in increasing order. */
  public static ImmutableList<Bid> bidRangeAfter(Bid after) {
    return bidRangeAfter(after, Bid.SEVEN_NOTRUMPS);
  }

  /** Returns all bids (after, high], in increasing order. */
  public static ImmutableList<Bid> bidRangeAfter(Bid after, Bid high) {
    return bidRange(Bid.forNumber(after.getNumber() + 1), high);
  }

  /** Returns all bids [low, high], in increasing order. */
  public static ImmutableList<Bid> bidRange(@Nullable Bid low, Bid high) {
    if (low == null) {
      checkArgument(high == Bid.SEVEN_NOTRUMPS, "[null, !7N] is invalid");
      return ImmutableList.of();
    }
    ImmutableList.Builder<Bid> builder = ImmutableList.builder();
    for (int i = low.getNumber(); i <= high.getNumber(); i++) {
      builder.add(Bid.forNumber(i));
    }
    return builder.build();
  }

  private static class BidPair {
    Level level;
    Suit suit;

    BidPair(Bid bid) {
      String name = bid.name();
      int index = name.indexOf('_');
      level = Level.valueOf(name.substring(0, index));
      String suitName = name.substring(index + 1);
      if (level == Level.ONE) {
        // singular forms
        suit = Suit.valueOf(suitName + "S");
      } else {
        suit = Suit.valueOf(suitName);
      }
    }
  }

  static {
    EnumMap<Bid, Level> bid2Level = Maps.newEnumMap(Bid.class);
    EnumMap<Bid, Suit> bid2Suit = Maps.newEnumMap(Bid.class);
    EnumMap<Level, EnumMap<Suit, Bid>> bidMap = Maps.newEnumMap(Level.class);

    for (Bid bid : Bid.values()) {
      BidPair bidPair = new BidPair(bid);
      bid2Level.put(bid, bidPair.level);
      bid2Suit.put(bid, bidPair.suit);
      EnumMap<Suit, Bid> suitMap = bidMap.get(bidPair.level);
      if (suitMap == null) {
        suitMap = newEnumMap(Suit.class);
        bidMap.put(bidPair.level, suitMap);
      }
      suitMap.put(bidPair.suit, bid);
    }
    BID_MAP = Maps.immutableEnumMap(bidMap);
    BID_TO_LEVEL = Maps.immutableEnumMap(bid2Level);
    BID_TO_SUIT = Maps.immutableEnumMap(bid2Suit);
  }
}
