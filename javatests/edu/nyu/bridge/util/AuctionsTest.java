package edu.nyu.bridge.util;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

import java.util.Arrays;

import org.junit.Test;

import edu.nyu.bridge.gen.Bridge.Auction;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.gen.Bridge.Auction.CallWithDescription;
import edu.nyu.bridge.gen.Bridge.Bid;
import edu.nyu.bridge.gen.Bridge.Call;

public class AuctionsTest {
  @Test
  public void testContract() {
    assertEquals("right contract", contract("2S", Direction.NORTH),
        Auctions.contract(auction(call("1S"), PASS, call("2S"), PASS)));
    assertEquals("right contract", contract("4H", Direction.EAST),
        Auctions.contract(
            auction(call("1S"), call("2H"), call("2S"), call("4H"), PASS, PASS, PASS)));
    assertEquals("right contract", contract("5C", Direction.WEST),
        Auctions.contract(auction(call("1S"), PASS, call("2S"), call("5C"))));
    assertEquals("right contract", contract("4HX", Direction.EAST),
        Auctions.contract(
            auction(call("1S"), call("2H"), call("2S"), call("4H"), call("X"), PASS, PASS, PASS)));

    assertEquals("right contract", contract("3C", Direction.SOUTH),
        Auctions.contract(auction(Direction.EAST, call("1D"), PASS, PASS, call("X"), call("P"),
            call("2C"), PASS, call("3C"), PASS, PASS, PASS)));
  }

  @Test
  public void testFancyContract() {
    assertEquals("right contract", contract("5S", Direction.EAST, Calls.DOUBLE),
        Auctions.contract(auction(call("1H"), call("1S"), PASS, call("3S"), call("4C"), call("4H"),
            call("4S"), PASS, call("5C"), call("5S"), DOUBLE, PASS, PASS, PASS)));
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
    assertFalse("passed out", Auctions.isPassout(auction(call("1S"), PASS, PASS, PASS)));
    assertTrue("all passed out", Auctions.isPassout(auction(PASS, PASS, PASS, PASS)));
  }

  @Test
  public void testOver() {
    assertTrue("passed out", Auctions.isOver(auction(call("1S"), PASS, PASS, PASS)));
    assertTrue("all passed out", Auctions.isOver(auction(PASS, PASS, PASS, PASS)));
    assertTrue(
        "passed out", Auctions.isOver(auction(call("1S"), PASS, call("2S"), PASS, PASS, PASS)));
  }

  @Test
  public void testNotOver() {
    assertFalse("live", Auctions.isOver(auction(call("1S"), PASS, PASS)));
    assertFalse("3 initial passes", Auctions.isOver(auction(PASS, PASS, PASS)));
    assertFalse("passed out",
        Auctions.isOver(auction(call("1S"), PASS, call("2S"), PASS, PASS, call("X"))));
  }

  @Test
  public void testDeclarer() {
    assertEquals("right declarer", contract("2s", "N"),
        Auctions.contract(auction(call("1S"), PASS, call("2S"), PASS, PASS, PASS)));

    assertEquals("right declarer", contract("4s", "W"),
        Auctions.contract(auction(call("1H"), PASS, call("2H"), call("4S"), PASS, PASS, PASS)));
  }

  @Test
  public void testIsValid() {
    CallWithDescription highBid = call("4S");
    Auction a = auction(call("1H"), PASS, call("2H"), highBid);
    for (Bid bid : Bid.values()) {
      if (bid.getNumber() <= highBid.getCall().getBid().getNumber()) {
        assertFalse("Can not bid " + bid, Auctions.isValid(a, call(bid)));
      } else {
        assertTrue("Can bid " + bid, Auctions.isValid(a, call(bid)));
      }
    }
    assertTrue("Can double opps", Auctions.isValid(a, call("X")));
    assertFalse("Can not redouble opps", Auctions.isValid(a, call("XX")));
    Auction aPass = a.toBuilder().addAuction(PASS).build();
    assertFalse("Can not double pard", Auctions.isValid(aPass, call("X")));
    assertFalse("Can not redouble w/o Dbl", Auctions.isValid(aPass, call("XX")));
    Auction aDbl = a.toBuilder().addAuction(call("X")).build();
    assertFalse("Can not double pard", Auctions.isValid(aDbl, call("X")));
    assertTrue("Can redouble after Dbl", Auctions.isValid(aDbl, call("XX")));
  }

  private void isRole(Auctions.Role role, Auction auction) {
    assertEquals("expected role from: " + auction, role, Auctions.nextRole(auction));
  }

  private Contract contract(String call, Direction declarer) {
    return new Contract(Contracts.string2Contract(call), declarer);
  }

  private Contract contract(String cont, String dir) {
    return new Contract(call(cont).getCall(), Directions.fromString(dir));
  }

  private Contract contract(String cont, Direction dir, Call risk) {
    Call c = call(cont).getCall().toBuilder().setNonBid(risk.getNonBid()).build();
    return new Contract(c, dir);
  }

  private static CallWithDescription call(Bid bid) {
    return CallWithDescription.newBuilder().setCall(Call.newBuilder().setBid(bid)).build();
  }

  private static CallWithDescription call(String call) {
    return CallWithDescription.newBuilder().setCall(Calls.string2Call(call)).build();
  }

  private static final CallWithDescription PASS = call("P");
  private static final CallWithDescription DOUBLE = call("X");

  private Auction auction(Direction dealer, CallWithDescription... calls) {
    return Auction.newBuilder().setDealer(dealer).addAllAuction(Arrays.asList(calls)).build();
  }

  private Auction auction(CallWithDescription... calls) {
    return auction(Direction.NORTH, calls);
  }
}
