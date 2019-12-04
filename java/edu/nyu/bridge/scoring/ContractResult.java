package edu.nyu.bridge.scoring;

import static com.google.common.base.Preconditions.checkArgument;

import com.google.auto.value.AutoValue;
import com.google.errorprone.annotations.Immutable;
import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.util.Calls;
import edu.nyu.bridge.util.Contract;

/** Wraps up a {@link Contract} and its {@link Result} tricks. */
@AutoValue
@Immutable
public abstract class ContractResult {
  public static ContractResult of(Contract contract, Result result) {
    return new AutoValue_ContractResult(contract, result);
  }

  public static ContractResult of(Bridge.Direction direction, Bridge.Bid bid, int tricks) {
    return of(Contract.of(Calls.bid2Call(bid), direction), Result.total(tricks));
  }

  /** {@link Contract} exactly making. */
  public static ContractResult making(Contract contract) {
    return making(contract, contract.getContractTricks());
  }

  /** {@link Contract} making the number of of contract tricks. e.g. {@code 4S making 5} */
  public static ContractResult making(Contract contract, int contractTricks) {
    checkArgument(
        contractTricks >= contract.getContractTricks(),
        "making %s < contract %s (maybe use down)",
        contractTricks,
        contract.getContractTricks());
    return of(contract, Result.total(Result.BOOK_TRICKS + contractTricks));
  }

  /** {@link Contract} down the number of of contract tricks. e.g. {@code 4S down 2} */
  public static ContractResult down(Contract contract, int underTricks) {
    int totalTricks = contract.getTotalTricks();
    int actualTricks = totalTricks - underTricks;
    checkArgument(
        totalTricks >= 0, "down %s exceeds available tricks %s", underTricks, totalTricks);
    return of(contract, Result.total(actualTricks));
  }

  /** {@link Contract} of the total number of of contract tricks. e.g. {@code 4S, 10 tricks} */
  public static ContractResult tricks(Contract contract, int totalTricks) {
    checkArgument(
        totalTricks >= 0 && totalTricks <= 13, "totalTricks %s out of range", totalTricks);
    return of(contract, Result.total(totalTricks));
  }

  public abstract Contract contract();

  /** number of tricks from perspective of declarer, e.g. 4S, 10 tricks. */
  public abstract Result result();

  /** Given the vulnerability, compute the {@link ContractScore}. */
  public ContractScore score(Bridge.Vulnerability vul) {
    return ContractScore.of(contract(), Scorer.score(contract(), vul, result()));
  }
}
