package edu.nyu.bridge.scoring;

import com.google.auto.value.AutoValue;

/** Represents a resulting number of tricks. */
@AutoValue
public abstract class Result {
  public static final int BOOK_TRICKS = 6;

  public static Result total(int tricks) {
    return new AutoValue_Result(tricks);
  }

  /** The total tricks, as in '10', rather than the 'contract tricks' which would be '4' */
  public abstract int getTotalTricks();

  public int getContractTricks() {
    return getTotalTricks() - BOOK_TRICKS;
  }
}
