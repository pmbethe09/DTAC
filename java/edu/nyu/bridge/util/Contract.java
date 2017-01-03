package edu.nyu.bridge.util;

import javax.annotation.Nullable;

import com.google.common.base.MoreObjects;
import com.google.common.base.Objects;
import com.google.common.base.Preconditions;

import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.gen.Bridge.Level;
import edu.nyu.bridge.gen.Bridge.NonBid;
import edu.nyu.cards.gen.Cards.Suit;

/** An intelligent wrapper for describing bridge contracts. */
public class Contract {
  public static final Contract PASSOUT =
      new Contract(Call.newBuilder().setNonBid(NonBid.PASS).build(), Direction.NORTH);

  public enum Bonus {
    PARTSCORE(50, 50),
    GAME(300, 500),
    SLAM(800, 1250),
    GRAND(1300, 2000);

    public final int nonVulBonus, vulBonus;

    Bonus(int nonVulBonus, int vulBonus) {
      this.nonVulBonus = nonVulBonus;
      this.vulBonus = vulBonus;
    }
  }

  public final Call call;
  public final Direction declarer;

  public Contract(Call call, Direction declarer) {
    this.call = Preconditions.checkNotNull(call, "call");
    this.declarer = Preconditions.checkNotNull(declarer, "declarer");
  }

  public boolean isPassout() {
    return !call.hasBid();
  }

  public Bonus bonus() {
    if (isGrand()) {
      return Bonus.GRAND;
    }
    if (isSlam()) {
      return Bonus.SLAM;
    }
    if (isGame()) {
      return Bonus.GAME;
    }
    return Bonus.PARTSCORE;
  }

  public boolean isGrand() {
    return Bids.level(call.getBid()).getNumber() == 7;
  }

  public boolean isSlam() {
    return Bids.level(call.getBid()).getNumber() == 6;
  }

  public boolean isGame() {
    Level level = Bids.level(call.getBid());
    switch (Bids.suit(call.getBid())) {
      case CLUBS:
      case DIAMONDS:
        return level.getNumber() == 5;
      case HEARTS:
      case SPADES:
        return level.getNumber() == 4 || level.getNumber() == 5;
      case NOTRUMPS:
        return level.getNumber() >= 3 && level.getNumber() < 6;
      default:
        throw new IllegalStateException("Suit " + Bids.suit(call.getBid()) + " returned for "
            + call.getBid() + " was not expected");
    }
  }

  @Override
  public String toString() {
    return MoreObjects.toStringHelper(this)
        .add("call", Contracts.contract2String(call))
        .add("declarer", declarer)
        .toString();
  }

  @Override
  public int hashCode() {
    return Objects.hashCode(this.call, this.declarer);
  }

  @Override
  public boolean equals(Object other) {
    if (!(other instanceof Contract)) {
      return false;
    }
    Contract contract = (Contract) other;
    return this.call.equals(contract.call) && this.declarer == contract.declarer;
  }

  /** returns trump or {@code null} if NT. */
  @Nullable
  public Suit getTrump() {
    Suit trump = Bids.suit(call.getBid());
    return trump == null || trump == Suit.NOTRUMPS ? null : trump;
  }

  /** Returns {@code true} if this beats current. */
  public boolean beats(@Nullable Contract current) {
    if (current == null) {
      return true;
    }
    int mLevel = Bids.level(this.call.getBid()).getNumber();
    int cLevel = Bids.level(current.call.getBid()).getNumber();
    if (mLevel > cLevel) {
      return true;
    }
    if (mLevel < cLevel) {
      return false;
    }
    return Bids.suit(this.call.getBid()).getNumber() > Bids.suit(current.call.getBid()).getNumber();
  }

  public String pbnString() {
    if (!this.call.hasBid()) {
      return "P";
    }
    return Contracts.contract2String(this.call) + declarer.name().substring(0, 1);
  }

  public static Contract parse(String c) {
    return new Contract(
        Contracts.string2Contract(c), Directions.fromString(c.substring(c.length() - 1)));
  }

  public static Contract of(int totalTricks, @Nullable Suit trump, Direction declarer) {
    Preconditions.checkArgument(totalTricks > BOOK, "Must make a contract with 7 or more tricks");
    trump = trump == null ? Suit.NOTRUMPS : trump;
    Call call =
        Call.newBuilder().setBid(Bids.bid(Level.forNumber(totalTricks - BOOK), trump)).build();
    return new Contract(call, declarer);
  }

  private static final int BOOK = 6;
}
