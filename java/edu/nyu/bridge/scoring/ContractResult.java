package edu.nyu.bridge.scoring;

import com.google.auto.value.AutoValue;
import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.util.Calls;
import edu.nyu.bridge.util.Contract;

/** Wraps up a {@link Contract} and its {@link Result} tricks. */
@AutoValue
public abstract class ContractResult {
  public static ContractResult of(Contract contract, Result result) {
    return new AutoValue_ContractResult(contract, result);
  }

  public static ContractResult of(Bridge.Direction direction, Bridge.Bid bid, int tricks) {
    return of(Contract.of(Calls.bid2Call(bid), direction), Result.total(tricks));
  }

  public abstract Contract contract();

  // number of tricks from perspective of declarer, e.g. 4S, 10 tricks.
  public abstract Result score();
}
