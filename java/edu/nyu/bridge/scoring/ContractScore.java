package edu.nyu.bridge.scoring;

import com.google.auto.value.AutoValue;
import edu.nyu.bridge.util.Contract;

/** Wraps up a {@link Contract} and its {@link Score}. */
@AutoValue
public abstract class ContractScore {
  public static ContractScore of(Contract contract, Score score) {
    return new AutoValue_ContractScore(contract, score);
  }

  public abstract Contract contract();

  public abstract Score score();
}
