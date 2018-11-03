package edu.nyu.bridge.scoring;

import com.google.common.annotations.VisibleForTesting;
import com.google.common.collect.ImmutableList;
import edu.nyu.bridge.gen.Bridge;
import java.util.Collections;

/** Computes the international matchpoint scores for a pair of {@link Score}. */
public final class Imper {
  private Imper() {}

  private static final ImmutableList<Integer> IMP_TABLE =
      ImmutableList.of(
          0, 20, 50, 90, 130, 170, 220, 270, 320, 370, 430, 500, 600, 750, 900, 1100, 1300, 1500,
          1750, 2000, 2250, 2500, 3000, 3500, 4000, 20000);

  /** Computes the imp-score from the perspective of table1. */
  public static ImpScore computeImps(Score table1, Score table2) {
    int rawScore = table1.absolute().getScore() - table2.absolute().getScore();
    return ImpScore.of(Score.of(Bridge.Direction.NORTH, rawImps(rawScore)));
  }

  @VisibleForTesting
  static int rawImps(int rawScore) {
    int idx = Collections.binarySearch(IMP_TABLE, Math.abs(rawScore));
    // returns '-insertion_point - 1' for non-exact matches, so convert.
    int posImps = idx < 0 ? -idx - 2 : idx;
    return rawScore < 0 ? -posImps : posImps;
  }
}
