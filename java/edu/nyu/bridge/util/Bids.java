package edu.nyu.bridge.util;

import static com.google.common.collect.Maps.newEnumMap;

import com.google.common.base.Preconditions;
import com.google.common.collect.ImmutableMap;
import com.google.common.collect.Maps;

import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Level;
import edu.nyu.cards.gen.Cards.Suit;

import java.util.EnumMap;

/** Provides helper methods for switching between {@link Bid} and {@link Suit}/{@link Level}. */
public class Bids {
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

  private static class BidPair {
    Level level;
    Suit suit;

    BidPair(Bid bid) {
      String name = bid.name();
      int index = name.indexOf('_');
      level = Preconditions.checkNotNull(Level.valueOf(name.substring(0, index)));
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
