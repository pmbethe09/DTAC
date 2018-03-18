package edu.nyu.bridge.util;

import static com.google.common.truth.Truth.assertThat;

import java.util.Arrays;

import org.junit.Test;

import edu.nyu.bridge.gen.Bridge.Auction;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Call;

public class AuctionsTest {
  @Test
  public void testContract() {
    assertThat(contract("2S", Direction.NORTH))
        .named("right contract")
        .isEqualTo(Auctions.contract(auction(call("1S"), PASS, call("2S"), PASS)));
    assertThat(contract("4H", Direction.EAST))
        .named("right contract")
        .isEqualTo(Auctions.contract(
            auction(call("1S"), call("2H"), call("2S"), call("4H"), PASS, PASS, PASS)));
    assertThat(contract("5C", Direction.WEST))
        .named("right contract")
        .isEqualTo(Auctions.contract(auction(call("1S"), PASS, call("2S"), call("5C"))));
    assertThat(contract("4HX", Direction.EAST))
        .named("right contract")
        .isEqualTo(Auctions.contract(
            auction(call("1S"), call("2H"), call("2S"), call("4H"), call("X"), PASS, PASS, PASS)));

    assertThat(contract("3C", Direction.SOUTH))
        .named("right contract")
        .isEqualTo(Auctions.contract(auction(Direction.EAST, call("1D"), PASS, PASS, call("X"),
            call("P"), call("2C"), PASS, call("3C"), PASS, PASS, PASS)));
  }

  @Test
  public void testFancyContract() {
    assertThat(contract("5S", Direction.EAST, Calls.DOUBLE))
        .named("right contract")
        .isEqualTo(Auctions.contract(auction(call("1H"), call("1S"), PASS, call("3S"), call("4C"),
            call("4H"), call("4S"), PASS, call("5C"), call("5S"), DOUBLE, PASS, PASS, PASS)));
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
    assertThat(Auctions.isPassout(auction(call("1S"), PASS, PASS, PASS)))
        .named("passed out")
        .isFalse();
    assertThat(Auctions.isPassout(auction(PASS, PASS, PASS, PASS)))
        .named("all passed out")
        .isTrue();
  }

  @Test
  public void testOver() {
    assertThat(Auctions.isOver(auction(call("1S"), PASS, PASS, PASS))).named("passed out").isTrue();
    assertThat(Auctions.isOver(auction(PASS, PASS, PASS, PASS))).named("all passed out").isTrue();
    assertThat(Auctions.isOver(auction(call("1S"), PASS, call("2S"), PASS, PASS, PASS)))
        .named("passed out")
        .isTrue();
  }

  @Test
  public void testNotOver() {
    assertThat(Auctions.isOver(auction(call("1S"), PASS, PASS))).named("live").isFalse();
    assertThat(Auctions.isOver(auction(PASS, PASS, PASS))).named("3 initial passes").isFalse();
    assertThat(Auctions.isOver(auction(call("1S"), PASS, call("2S"), PASS, PASS, call("X"))))
        .named("passed out")
        .isFalse();
  }

  @Test
  public void testDeclarer() {
    assertThat(contract("2s", "N"))
        .named("right declarer")
        .isEqualTo(Auctions.contract(auction(call("1S"), PASS, call("2S"), PASS, PASS, PASS)));

    assertThat(contract("4s", "W"))
        .named("right declarer")
        .isEqualTo(
            Auctions.contract(auction(call("1H"), PASS, call("2H"), call("4S"), PASS, PASS, PASS)));
  }

  @Test
  public void testIsValid() {
    Call highBid = call("4S");
    Auction a = auction(call("1H"), PASS, call("2H"), highBid);
    for (Bid bid : Bid.values()) {
      if (bid.getNumber() <= highBid.getBid().getNumber()) {
        assertThat(Auctions.isValid(a, call(bid))).named("Can not bid %s", bid).isFalse();
      } else {
        assertThat(Auctions.isValid(a, call(bid))).named("Can bid %s", bid).isTrue();
      }
    }
    assertThat(Auctions.isValid(a, call("X"))).named("Can double opps").isTrue();
    assertThat(Auctions.isValid(a, call("XX"))).named("Can not redouble opps").isFalse();
    Auction aPass = a.toBuilder().addAuction(PASS).build();
    assertThat(Auctions.isValid(aPass, call("X"))).named("Can not double pard").isFalse();
    assertThat(Auctions.isValid(aPass, call("XX"))).named("Can not redouble w/o Dbl").isFalse();
    Auction aDbl = a.toBuilder().addAuction(call("X")).build();
    assertThat(Auctions.isValid(aDbl, call("X"))).named("Can not double pard").isFalse();
    assertThat(Auctions.isValid(aDbl, call("XX"))).named("Can redouble after Dbl").isTrue();
  }

  private void isRole(Auctions.Role role, Auction auction) {
    assertThat(role).named("expected role from: %s", auction).isEqualTo(Auctions.nextRole(auction));
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
