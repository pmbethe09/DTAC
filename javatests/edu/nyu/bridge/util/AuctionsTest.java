package edu.nyu.bridge.util;

import static com.google.common.collect.ImmutableSet.toImmutableSet;
import static com.google.common.truth.Truth.assertThat;
import static edu.nyu.bridge.util.Bids.bidRange;
import static edu.nyu.bridge.util.Bids.bidRangeAfter;

import com.google.common.collect.ImmutableList;
import com.google.common.collect.ImmutableSet;
import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.gen.Bridge.Auction;
import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.Direction;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import org.junit.Test;

public class AuctionsTest {
  private static final ImmutableSet<Bridge.NonBid> ONLY_PASS = ImmutableSet.of(Bridge.NonBid.PASS);
  private static final ImmutableSet<Bridge.NonBid> CAN_DOUBLE =
      ImmutableSet.of(Bridge.NonBid.PASS, Bridge.NonBid.DOUBLE);
  private static final ImmutableSet<Bridge.NonBid> CAN_REDOUBLE =
      ImmutableSet.of(Bridge.NonBid.PASS, Bridge.NonBid.REDOUBLE);

  @Test
  public void legalBids() {
    runLegalBids(bidRangeAfter(Bid.SEVEN_NOTRUMPS), CAN_DOUBLE, call("7N"));
    runLegalBids(bidRange(Bid.ONE_CLUB, Bid.SEVEN_NOTRUMPS), ONLY_PASS);

    runLegalBids(bidRangeAfter(Bid.TWO_SPADES), ONLY_PASS, call("1S"), PASS, call("2S"), PASS);
    runLegalBids(bidRangeAfter(Bid.TWO_SPADES), CAN_DOUBLE, call("1S"), PASS, call("2S"));
    runLegalBids(
        bidRangeAfter(Bid.TWO_SPADES), CAN_REDOUBLE, call("1S"), PASS, call("2S"), Calls.DOUBLE);
    runLegalBids(
        bidRangeAfter(Bid.TWO_SPADES), ONLY_PASS, call("1S"), PASS, call("2S"), Calls.DOUBLE, PASS);
    runLegalBids(
        bidRangeAfter(Bid.TWO_SPADES),
        CAN_REDOUBLE,
        call("1S"),
        PASS,
        call("2S"),
        Calls.DOUBLE,
        PASS,
        PASS);
    runLegalBids(
        bidRangeAfter(Bid.TWO_SPADES),
        ONLY_PASS,
        call("1S"),
        PASS,
        call("2S"),
        Calls.DOUBLE,
        PASS,
        PASS,
        Calls.REDOUBLE);
    runLegalBids(
        bidRangeAfter(Bid.TWO_SPADES),
        ONLY_PASS,
        call("1S"),
        PASS,
        call("2S"),
        Calls.DOUBLE,
        PASS,
        PASS,
        Calls.REDOUBLE,
        PASS);
    runLegalBids(
        bidRangeAfter(Bid.TWO_SPADES),
        ONLY_PASS,
        call("1S"),
        PASS,
        call("2S"),
        Calls.DOUBLE,
        PASS,
        PASS,
        Calls.REDOUBLE,
        PASS,
        PASS);
  }

  private void runLegalBids(
      List<Bid> expectedBids, Set<Bridge.NonBid> expectedNonBids, Call... calls) {
    ImmutableList<Call> legalCalls = Auctions.legalBids(auction(calls));
    // confirms that only bid xor non-bid set
    legalCalls.forEach(c -> assertThat(c.hasBid() ^ c.hasNonBid()).isTrue());
    Set<Bridge.NonBid> nonBids =
        legalCalls.stream().filter(Call::hasNonBid).map(Call::getNonBid).collect(toImmutableSet());
    Set<Bid> bids =
        legalCalls.stream().filter(Call::hasBid).map(Call::getBid).collect(toImmutableSet());
    assertThat(nonBids).containsExactlyElementsIn(expectedNonBids);
    assertThat(bids).containsExactlyElementsIn(expectedBids);
  }

  @Test
  public void testContract() {
    assertThat(contract("2S", Direction.NORTH))
        .isEqualTo(Auctions.contract(auction(call("1S"), PASS, call("2S"), PASS)));
    assertThat(contract("4H", Direction.EAST))
        .isEqualTo(
            Auctions.contract(
                auction(call("1S"), call("2H"), call("2S"), call("4H"), PASS, PASS, PASS)));
    assertThat(contract("5C", Direction.WEST))
        .isEqualTo(Auctions.contract(auction(call("1S"), PASS, call("2S"), call("5C"))));
    assertThat(contract("4HX", Direction.EAST))
        .isEqualTo(
            Auctions.contract(
                auction(
                    call("1S"), call("2H"), call("2S"), call("4H"), call("X"), PASS, PASS, PASS)));

    assertThat(contract("3C", Direction.SOUTH))
        .isEqualTo(
            Auctions.contract(
                auction(
                    Direction.EAST,
                    call("1D"),
                    PASS,
                    PASS,
                    call("X"),
                    call("P"),
                    call("2C"),
                    PASS,
                    call("3C"),
                    PASS,
                    PASS,
                    PASS)));
  }

  @Test
  public void testFancyContract() {
    assertThat(contract("5S", Direction.EAST, Calls.DOUBLE))
        .isEqualTo(
            Auctions.contract(
                auction(
                    call("1H"),
                    call("1S"),
                    PASS,
                    call("3S"),
                    call("4C"),
                    call("4H"),
                    call("4S"),
                    PASS,
                    call("5C"),
                    call("5S"),
                    DOUBLE,
                    PASS,
                    PASS,
                    PASS)));
  }

  @Test
  public void testRoles() {
    isRole(Auctions.Role.OPENER, auction());
    isRole(Auctions.Role.OPENER, auction(PASS));
    isRole(Auctions.Role.OPENER, auction(PASS, PASS));
    isRole(Auctions.Role.RESPONDER, auction(call("1D"), PASS));
    isRole(Auctions.Role.OVERCALLER, auction(call("1D")));
    isRole(Auctions.Role.OVERCALLER, auction(call("1D"), PASS, call("2D")));
    isRole(Auctions.Role.ADVANCER, auction(call("1D"), call("X"), call("2D")));
    isRole(Auctions.Role.OPENER, auction(call("1D"), PASS, call("2D"), PASS));
    isRole(Auctions.Role.OVERCALLER, auction(call("1D"), PASS, call("2D"), PASS, call("2N")));
    isRole(Auctions.Role.RESPONDER, auction(call("1D"), PASS, call("2D"), PASS, call("2N"), PASS));
  }

  @Test
  public void testPassout() {
    assertThat(Auctions.isPassout(auction(call("1S"), PASS, PASS, PASS))).isFalse();
    assertThat(Auctions.isPassout(auction(PASS, PASS, PASS, PASS))).isTrue();
  }

  @Test
  public void testOver() {
    assertThat(Auctions.isOver(auction(call("1S"), PASS, PASS, PASS))).isTrue();
    assertThat(Auctions.isOver(auction(PASS, PASS, PASS, PASS))).isTrue();
    assertThat(Auctions.isOver(auction(call("1S"), PASS, call("2S"), PASS, PASS, PASS))).isTrue();
  }

  @Test
  public void testNotOver() {
    assertThat(Auctions.isOver(auction(call("1S"), PASS, PASS))).isFalse();
    assertThat(Auctions.isOver(auction(PASS, PASS, PASS))).isFalse();
    assertThat(Auctions.isOver(auction(call("1S"), PASS, call("2S"), PASS, PASS, call("X"))))
        .isFalse();
  }

  @Test
  public void testDeclarer() {
    assertThat(contract("2s", "N"))
        .isEqualTo(Auctions.contract(auction(call("1S"), PASS, call("2S"), PASS, PASS, PASS)));

    assertThat(contract("4s", "W"))
        .isEqualTo(
            Auctions.contract(auction(call("1H"), PASS, call("2H"), call("4S"), PASS, PASS, PASS)));
  }

  @Test
  public void testIsValid() {
    Call highBid = call("4S");
    Auction a = auction(call("1H"), PASS, call("2H"), highBid);
    for (Bid bid : Bid.values()) {
      if (bid.getNumber() <= highBid.getBid().getNumber()) {
        assertThat(Auctions.isValid(a, call(bid))).isFalse();
      } else {
        assertThat(Auctions.isValid(a, call(bid))).isTrue();
      }
    }
    assertThat(Auctions.isValid(a, call("X"))).isTrue();
    assertThat(Auctions.isValid(a, call("XX"))).isFalse();
    Auction aPass = a.toBuilder().addAuction(PASS).build();
    assertThat(Auctions.isValid(aPass, call("X"))).isFalse();
    assertThat(Auctions.isValid(aPass, call("XX"))).isFalse();
    Auction aDbl = a.toBuilder().addAuction(call("X")).build();
    assertThat(Auctions.isValid(aDbl, call("X"))).isFalse();
    assertThat(Auctions.isValid(aDbl, call("XX"))).isTrue();
  }

  private void isRole(Auctions.Role role, Auction auction) {
    assertThat(role).isEqualTo(Auctions.nextRole(auction));
  }

  private Contract contract(String call, Direction declarer) {
    return Contract.of(Contracts.string2Contract(call), declarer);
  }

  private Contract contract(String cont, String dir) {
    return Contract.of(call(cont), Directions.fromString(dir));
  }

  private Contract contract(String cont, Direction dir, Call risk) {
    Call c = call(cont).toBuilder().setNonBid(risk.getNonBid()).build();
    return Contract.of(c, dir);
  }

  private static Call call(Bid bid) {
    return Call.newBuilder().setBid(bid).build();
  }

  private static Call call(String call) {
    return Calls.string2Call(call);
  }

  private static final Call PASS = call("P");
  private static final Call DOUBLE = call("X");

  private Auction auction(Direction dealer, Call... calls) {
    return Auction.newBuilder().setDealer(dealer).addAllAuction(Arrays.asList(calls)).build();
  }

  private Auction auction(Call... calls) {
    return auction(Direction.NORTH, calls);
  }
}
