package edu.nyu.bridge.scoring;

import static edu.nyu.bridge.util.Directions.isNS;

import com.google.auto.value.AutoValue;
import com.google.errorprone.annotations.Immutable;
import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.util.Contract;

/** Represents a resulting number of tricks. */
@AutoValue
@Immutable
public abstract class ScoreResult {
  ScoreResult() {}

  public static ScoreResult of(Contract contract, Score score, Result result) {
    return new AutoValue_ScoreResult(contract, score, result);
  }

  public static ScoreResult infer(ContractScore score, Bridge.Vulnerability vul) {
    Score fromDeclarer =
        isNS(score.contract().getDeclarer()) ? score.score().absolute() : score.score().forEW();
    if (fromDeclarer.getScore() > 0) {
      // making or more
      for (int t = score.contract().getContractTricks(); t <= 7; t++) {
        ContractResult result = ContractResult.making(score.contract(), t);
        if (result.score(vul).score().equals(score.score())) {
          return of(score.contract(), score.score(), result.result());
        }
      }
    } else {
      for (int u = 1; u <= score.contract().getTotalTricks(); u++) {
        ContractResult result = ContractResult.down(score.contract(), u);
        if (result.score(vul).score().equals(score.score())) {
          return of(score.contract(), score.score(), result.result());
        }
      }
    }
    throw new IllegalArgumentException(
        String.format("unable to match a result to %s %s", score, vul));
  }

  public abstract Contract contract();

  public abstract Score score();

  public abstract Result result();
}
