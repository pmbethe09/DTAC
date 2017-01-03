package edu.nyu.bridge.util;

import javax.annotation.Nullable;

import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.NonBid;

/**
 * Common conversions from string<-> contract [reusing Call proto]
 */
public class Contracts {
  @Nullable
  public static Contract highest(Iterable<Contract> from) {
    Contract highest = null;
    for (Contract contract : from) {
      if (contract.beats(highest)) {
        highest = contract;
      }
    }
    return highest;
  }

  public static String contract2String(@Nullable Call contract) {
    if (contract == null) {
      return "P";
    }
    return Calls.call2String(contract) + call2Risk(contract);
  }

  public static Call string2Contract(String contract) {
    Call.Builder result = Calls.string2Call(contract).toBuilder();
    NonBid risk = string2Risk(contract);
    if (risk != NonBid.PASS) {
      result.setNonBid(risk);
    }
    return result.build();
  }

  private static NonBid string2Risk(String call) {
    if (call.endsWith("XX")) {
      return NonBid.REDOUBLE;
    }
    if (call.endsWith("X")) {
      return NonBid.DOUBLE;
    }
    return NonBid.PASS;
  }

  private static String call2Risk(Call call) {
    switch (call.getNonBid()) {
      case PASS:
        return "";
      case DOUBLE:
        return "X";
      case REDOUBLE:
        return "XX";
      default:
        throw new AssertionError("Unexpected value: " + call.getNonBid());
    }
  }
}
