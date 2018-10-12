package edu.nyu.bridge.scoring;

import com.google.auto.value.AutoValue;
import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.util.Calls;
import edu.nyu.bridge.util.Contract;

/** Wraps up a {@link Contract} and its {@link Score}. */
@AutoValue
public abstract class ContractScore {
  public static ContractScore of(Contract contract, Score score) {
    return new AutoValue_ContractScore(contract, score);
  }

  public static ContractScore of(Bridge.Direction direction, Bridge.Bid bid, int score) {
    return of(Contract.of(Calls.bid2Call(bid), direction), Score.of(direction, score));
  }

  public abstract Contract contract();

  public abstract Score score();
}
