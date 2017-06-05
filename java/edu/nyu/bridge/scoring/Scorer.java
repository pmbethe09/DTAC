package edu.nyu.bridge.scoring;

import edu.nyu.bridge.util.Contract;
import edu.nyu.bridge.gen.Bridge.Vulnerability;

public class Scorer {
  /** Returns NS-relative score. */
  public static int score(Contract contract, Vulnerability vul, int totalTricks) {
    if (contract.isPassout()) {
      return 0;
    }
    return 42; // TODO
  }
}