package edu.nyu.bridge.util;

import edu.nyu.bridge.gen.Bridge;

public class Vulnerabilities {
  public static boolean isVul(Bridge.Direction dir, Bridge.Vulnerability vul) {
    switch (vul) {
      case NONE:
        return false;
      case BOTH:
        return true;
      case NS:
        return Directions.isNS(dir);
      case EW:
        return !Directions.isNS(dir);
    }
    throw new IllegalArgumentException("Unexpected vul: " + vul);
  }
}
