package edu.nyu.bridge.util;

import static com.google.common.collect.Maps.newEnumMap;
import static com.google.common.collect.Maps.newHashMap;

import java.util.EnumMap;
import java.util.Map;

import javax.annotation.Nullable;

import com.google.common.collect.ImmutableMap;
import com.google.common.collect.Maps;

import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.NonBid;
import edu.nyu.cards.gen.Cards.Suit;

/**
 * Static methods for converting string to {@link Call} and back.
 */
public class Calls {
  private Calls() {}

  public static final Call PASS = Call.newBuilder().setNonBid(NonBid.PASS).build();
  public static final Call DOUBLE = Call.newBuilder().setNonBid(NonBid.DOUBLE).build();
  public static final Call REDOUBLE = Call.newBuilder().setNonBid(NonBid.REDOUBLE).build();

  public static Call raw(Call call) {
    return call.toBuilder().clearAlerted().clearDescription().build();
  }
  
  public static boolean equals(Call lhs, Call rhs) {
    return raw(lhs).equals(raw(rhs));
  }
  
  public static Call withDescription(Call call) {
    return withDescription(call, null);
  }

  public static Call withDescription(Call call, @Nullable String description) {
    Call.Builder builder = call.toBuilder();
    if (description != null) {
      builder.setDescription(description);
    }
    return builder.build();
  }

  public static String call2String(Call call) {
    if (call.hasBid()) {
      return BID_TO_STRING.get(call.getBid());
    }
    return NON_BID_TO_STRING.get(call.getNonBid());
  }

  public static Call string2Call(String bid) {
    Call call = STRING_TO_CALL.get(bid.toUpperCase());
    if (call != null) {
      return call;
    }
    return STRING_TO_CALL.get(bid.toUpperCase().substring(0, 2));
  }

  public static Call bid2Call(Bid bid) {
    return BID_TO_CALL.get(bid);
  }

  /**
   * Returns {@code true} if the next bid is allowed after the previous one.
   *
   * Note: it is up to the caller to ask the right question.
   * e.g. 1H Pass 1C -- if the caller asks if 1C is legal after Pass, the answer is true
   * but after 1H, not so much.
   */
  public static boolean allowed(@Nullable Call previous, Call next) {
    // TODO: this could be wrong if prev was a pass
    if (previous == null || !previous.hasBid()) {
      return true;
    }
    if (!next.hasBid()) {
      return true;
    }
    Bid prevBid = previous.getBid();
    Bid nextBid = next.getBid();
    return nextBid.getNumber() > prevBid.getNumber();
  }

  private enum CallStrings {
    PASS(Calls.PASS, "p", "pass"),
    REDOUBLE(Calls.REDOUBLE, "xx", "redouble", "rdbl", "r"),
    DOUBLE(Calls.DOUBLE, "x", "dbl", "double", "d");

    private final Call call;
    private final String[] aliases;

    CallStrings(Call call, String... aliases) {
      this.call = call;
      this.aliases = aliases;
    }
  }

  private static final ImmutableMap<Bid, Call> BID_TO_CALL;
  private static final ImmutableMap<Bid, String> BID_TO_STRING;
  private static final ImmutableMap<String, Call> STRING_TO_CALL;
  private static final ImmutableMap<NonBid, String> NON_BID_TO_STRING;

  static {
    Map<String, Call> string2Call = newHashMap();
    EnumMap<NonBid, String> nonBid2String = newEnumMap(NonBid.class);
    for (CallStrings callString : CallStrings.values()) {
      Call call = callString.call;
      for (String alias : callString.aliases) {
        string2Call.put(alias.toUpperCase(), call);
      }
      // use first string as output.
      nonBid2String.put(call.getNonBid(), callString.aliases[0].toUpperCase());
    }
    NON_BID_TO_STRING = Maps.immutableEnumMap(nonBid2String);

    EnumMap<Bid, Call> bid2Call = newEnumMap(Bid.class);
    EnumMap<Bid, String> bid2String = newEnumMap(Bid.class);

    for (Bid bid : Bid.values()) {
      Call call = Call.newBuilder().setBid(bid).build();
      Suit suit = Bids.suit(bid);
      String stringRep = "" + Bids.level(bid).getNumber() + suit.name().substring(0, 1);
      string2Call.put(stringRep, call);
      bid2Call.put(bid, call);
      bid2String.put(bid, stringRep);
    }
    BID_TO_CALL = Maps.immutableEnumMap(bid2Call);
    BID_TO_STRING = Maps.immutableEnumMap(bid2String);
    STRING_TO_CALL = ImmutableMap.copyOf(string2Call);
  }
}
