package edu.nyu.bridge.util;

import com.google.auto.value.AutoValue;
import com.google.common.base.Preconditions;
import com.google.errorprone.annotations.Immutable;
import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.gen.Bridge.Level;
import edu.nyu.bridge.gen.Bridge.NonBid;
import edu.nyu.cards.gen.Cards.Suit;
import javax.annotation.Nullable;

/** An intelligent wrapper for describing bridge contracts. */
@AutoValue
@Immutable
public abstract class Contract {
  public static final Contract PASSOUT =
      Contract.of(Call.newBuilder().setNonBid(NonBid.PASS).build(), Direction.NORTH);

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

  public static Contract of(Call call, Direction declarer) {
    return new AutoValue_Contract(call, declarer);
  }

  public static Contract parse(String c) {
    return Contract.of(
        Contracts.string2Contract(c.substring(0, c.length() - 1)),
        Directions.fromString(c.substring(c.length() - 1)));
  }

  public static Contract of(int totalTricks, @Nullable Suit trump, Direction declarer) {
    Preconditions.checkArgument(totalTricks > BOOK, "Must make a contract with 7 or more tricks");
    trump = trump == null ? Suit.NOTRUMPS : trump;
    Call call =
        Call.newBuilder().setBid(Bids.bid(Level.forNumber(totalTricks - BOOK), trump)).build();
    return Contract.of(call, declarer);
  }

  Contract() {}

  public abstract Call getCall();

  public abstract Direction getDeclarer();

  public boolean isPassout() {
    return !getCall().hasBid();
  }

  /** Returns a copy of this contract but {@code DOUBLED}. */
  public Contract doubled() {
    switch (getCall().getNonBid()) {
      case DOUBLE:
        return this;
      case PASS:
        return of(getCall().toBuilder().setNonBid(NonBid.DOUBLE).build(), getDeclarer());
      case REDOUBLE:
        throw new IllegalStateException("can't double a redoubled");
    }
    throw new AssertionError("switch not exhaustive");
  }

  /** Returns a copy of this contract but {@code REDOUBLED}. */
  public Contract redoubled() {
    switch (getCall().getNonBid()) {
      case REDOUBLE:
        return this;
      case PASS:
        // fallthrough
      case DOUBLE:
        return of(getCall().toBuilder().setNonBid(NonBid.REDOUBLE).build(), getDeclarer());
    }
    throw new AssertionError("switch not exhaustive");
  }

  /** Returns a copy of this contract but at the new level, wipes out risk. */
  public Contract atLevel(Bridge.Level level) {
    if (level == Bids.level(getCall().getBid())) {
      return this;
    }
    return of(level.getNumber() + BOOK, getTrump(), getDeclarer());
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
    return Bids.level(getCall().getBid()).getNumber() == 7;
  }

  public boolean isSlam() {
    return Bids.level(getCall().getBid()).getNumber() == 6;
  }

  public boolean isGame() {
    Level level = Bids.level(getCall().getBid());
    switch (Bids.suit(getCall().getBid())) {
      case CLUBS:
      case DIAMONDS:
        return level.getNumber() == 5;
      case HEARTS:
      case SPADES:
        return level.getNumber() == 4 || level.getNumber() == 5;
      case NOTRUMPS:
        return level.getNumber() >= 3 && level.getNumber() < 6;
      default:
        throw new IllegalStateException(
            "Suit "
                + Bids.suit(getCall().getBid())
                + " returned for "
                + getCall().getBid()
                + " was not expected");
    }
  }

  public int getContractTricks() {
    if (!getCall().hasBid()) {
      return 0;
    }
    return Bids.level(getCall().getBid()).getNumber();
  }

  public int getTotalTricks() {
    int ct = getContractTricks();
    return ct > 0 ? ct + BOOK : 0;
  }

  /** returns trump or {@code null} if NT. */
  @Nullable
  public Suit getTrump() {
    Suit trump = Bids.suit(getCall().getBid());
    return trump == null || trump == Suit.NOTRUMPS ? null : trump;
  }

  /** Returns {@code true} if this beats current. */
  public boolean beats(@Nullable Contract current) {
    if (current == null) {
      return true;
    }
    int mLevel = Bids.level(this.getCall().getBid()).getNumber();
    int cLevel = Bids.level(current.getCall().getBid()).getNumber();
    if (mLevel > cLevel) {
      return true;
    }
    if (mLevel < cLevel) {
      return false;
    }
    return Bids.suit(this.getCall().getBid()).getNumber()
        > Bids.suit(current.getCall().getBid()).getNumber();
  }

  public String pbnString() {
    if (!this.getCall().hasBid()) {
      return "P";
    }
    return Contracts.contract2String(this.getCall()) + getDeclarer().name().substring(0, 1);
  }

  private static final int BOOK = 6;
}
