package edu.nyu.bridge.scoring;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.util.Contract;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public final class ScoreResultTest {
  @Test
  public void testInfer() {
    assertThat(
            ScoreResult.infer(
                    ContractResult.down(Contract.parse("5SXN"), 3).score(Bridge.Vulnerability.NONE),
                    Bridge.Vulnerability.NONE)
                .result())
        .isEqualTo(Result.total(8));
  }
}
