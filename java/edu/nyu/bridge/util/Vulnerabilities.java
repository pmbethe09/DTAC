package edu.nyu.bridge.util;

import com.google.common.collect.ImmutableMap;
import edu.nyu.bridge.gen.Bridge;

import static com.google.common.base.Preconditions.checkArgument;

public final class Vulnerabilities {
  private Vulnerabilities() {}

  private static final ImmutableMap<String, Bridge.Vulnerability> VUL_TO_STRING =
      ImmutableMap.<String, Bridge.Vulnerability>builder()
          .put("none", Bridge.Vulnerability.NONE)
          // Note that n -> NS, not none
          .put("n", Bridge.Vulnerability.NS)
          .put("ns", Bridge.Vulnerability.NS)
          .put("north", Bridge.Vulnerability.NS)
          .put("e", Bridge.Vulnerability.EW)
          .put("ew", Bridge.Vulnerability.EW)
          .put("east", Bridge.Vulnerability.EW)
          .put("all", Bridge.Vulnerability.BOTH)
          .put("both", Bridge.Vulnerability.BOTH)
          .put("b", Bridge.Vulnerability.BOTH)
          .build();

  public static Bridge.Vulnerability fromString(String vul) {
    Bridge.Vulnerability res = VUL_TO_STRING.get(vul.toLowerCase());
    checkArgument(res != null, "no Vul mapping for %s", vul);
    return res;
  }

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
