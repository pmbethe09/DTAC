package edu.nyu.bridge.util;

import com.google.common.base.Preconditions;
import com.google.common.collect.Maps;
import edu.nyu.bridge.gen.Bridge.Auction;
import edu.nyu.bridge.gen.Bridge.AuctionOrBuilder;
import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.gen.Bridge.NonBid;
import edu.nyu.cards.gen.Cards.Suit;
import java.util.EnumMap;
import javax.annotation.Nullable;

/** Helper methods for acting on an {@link Auction}. */
public final class Auctions {
  private Auctions() {}

  /** Represents the role each player takes in an Auction. */
  public enum Role {
    /** The first to bid. */
    OPENER,
    /** The partner of the OPENER. */
    RESPONDER,
    /** The opponent of the OPENER who is first not to Pass. */
    OVERCALLER,
    /** The partner of the OVERCALLER. */
    ADVANCER;
  }

  /** Returns the role of the player next to bid. */
  public static Role nextRole(AuctionOrBuilder auction) {
    Direction opener = opener(auction, auction.getDealer());
    if (opener == null) {
      return Role.OPENER;
    }
    Direction me = Directions.add(auction.getDealer(), auction.getAuctionCount());
    if (opener == me) {
      return Role.OPENER;
    }
    if (opener == Directions.partner(me)) {
      return Role.RESPONDER;
    }
    Direction result = auction.getDealer();
    for (Call cwd : auction.getAuctionList()) {
      if (cwd.hasBid() || cwd.getNonBid() != NonBid.PASS) {
        if (result == me) {
          return Role.OVERCALLER;
        }
        if (result == Directions.partner(me)) {
          return Role.ADVANCER;
        }
      }
      result = Directions.lho(result);
    }
    // No actions yet by opps to bidding side
    return Role.OVERCALLER;
  }

  /** Returns the first player to bid in an auction or if none then {@code null}. */
  @Nullable
  public static Direction opener(AuctionOrBuilder builder, Direction direction) {
    Direction result = direction;
    for (Call cwd : builder.getAuctionList()) {
      if (cwd.hasBid()) {
        return result;
      }
      result = Directions.lho(result);
    }
    return null;
  }

  public static Auction.Builder start(Direction dir) {
    return Auction.newBuilder().setDealer(dir);
  }

  public static Direction openingLeader(AuctionOrBuilder auction) {
    return openingLeader(contract(auction));
  }

  public static Direction openingLeader(Contract contract) {
    return Directions.lho(contract.getDeclarer());
  }

  /** Returns the final {@link Contract} of an {@link Auction}. */
  public static Contract contract(AuctionOrBuilder auction) {
    if (isPassout(auction)) {
      return Contract.PASSOUT;
    }
    Call.Builder result = Call.newBuilder();
    Direction declarer = null;
    EnumMap<Suit, Direction> firstBidderNS = Maps.newEnumMap(Suit.class);
    EnumMap<Suit, Direction> firstBidderEW = Maps.newEnumMap(Suit.class);
    Direction bidder = auction.getDealer();
    for (Call call : auction.getAuctionList()) {
      if (call.hasBid()) {
        Suit suit = Bids.suit(call.getBid());
        if (Directions.isNS(bidder)) {
          if (!firstBidderNS.containsKey(suit)) {
            firstBidderNS.put(suit, bidder);
          }
        } else {
          if (!firstBidderEW.containsKey(suit)) {
            firstBidderEW.put(suit, bidder);
          }
        }
        result.setBid(call.getBid()).clearNonBid();
        declarer = bidder;
      }
      if (call.hasNonBid()) {
        switch (call.getNonBid()) {
          case REDOUBLE:
          case DOUBLE:
            result.setNonBid(call.getNonBid());
            break;
          default:
            break;
        }
      }
      bidder = Directions.lho(bidder);
    }
    Preconditions.checkArgument(
        result.hasBid(), "Unable to find the dominant bid in a non passout %s", auction);
    Suit suit = Bids.suit(result.getBid());
    if (Directions.isNS(declarer)) {
      return Contract.of(result.build(), firstBidderNS.get(suit));
    }
    return Contract.of(result.build(), firstBidderEW.get(suit));
  }

  public static boolean isOver(AuctionOrBuilder auction) {
    if (auction.getAuctionCount() <= 3) {
      return false;
    }
    // check that last 3 calls were Passes
    for (int i = auction.getAuctionCount() - 3; i < auction.getAuctionCount(); ++i) {
      if (!isPass(auction.getAuction(i))) {
        return false;
      }
    }
    return true;
  }

  public static boolean isPass(Call call) {
    return !call.hasBid() && call.getNonBid() == NonBid.PASS;
  }

  public static boolean isPassout(AuctionOrBuilder auction) {
    return auction.getAuctionCount() == 4 && isPass(auction.getAuction(0)) && isOver(auction);
  }

  /** Returns {@code true} if the given call is a valid one at this point in the given auction. */
  public static boolean isValid(AuctionOrBuilder auction, Call call) {
    if (call.hasBid()) {
      for (int i = auction.getAuctionCount() - 1; i >= 0; --i) {
        Call prev = auction.getAuction(i);
        if (prev.hasBid()) {
          return Calls.allowed(prev, call);
        }
      }
      return true; // no prev calls, so sure.
    }
    if (call.getNonBid() == NonBid.PASS) {
      return true;
    }
    // TODO perhaps more efficient?
    Contract contract = contract(auction);
    if (!contract.getCall().hasBid()) {
      return false; // can't X/XX a nothing.
    }
    Direction me = Directions.add(auction.getDealer(), auction.getAuctionCount());
    if (Directions.opponents(me, contract.getDeclarer())) {
      return contract.getCall().getNonBid() == NonBid.PASS && call.getNonBid() == NonBid.DOUBLE;
    }
    return contract.getCall().getNonBid() == NonBid.DOUBLE && call.getNonBid() == NonBid.REDOUBLE;
  }
}
