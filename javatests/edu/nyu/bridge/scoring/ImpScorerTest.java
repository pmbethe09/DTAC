package edu.nyu.bridge.scoring;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.util.Contract;
import org.junit.Test;

public class ImpScorerTest {

  @Test
  public void testEndToEnd() {
    assertThat(
            Imper.computeImps(
                ContractResult.tricks(Contract.parse("7NxxN"), 0)
                    .score(Bridge.Vulnerability.BOTH)
                    .score(),
                ContractResult.tricks(Contract.parse("7NxxE"), 0)
                    .score(Bridge.Vulnerability.BOTH)
                    .score()))
        .isEqualTo(ImpScore.of(Direction.NORTH, -24));
  }

  @Test
  public void testBasic() {
    assertThat(Imper.computeImps(Score.of(Direction.SOUTH, 90), Score.of(Direction.SOUTH, 100)))
        .isEqualTo(ImpScore.of(Direction.NORTH, 0));
    assertThat(Imper.computeImps(Score.of(Direction.SOUTH, 90), Score.of(Direction.EAST, 620)))
        .isEqualTo(ImpScore.of(Direction.EAST, -12));
    assertThat(Imper.computeImps(Score.of(Direction.EAST, 620), Score.of(Direction.SOUTH, 90)))
        .isEqualTo(ImpScore.of(Direction.EAST, 12));
  }

  @Test
  public void testRawImps() {
    assertThat(Imper.rawImps(0)).isEqualTo(0);
    assertThat(Imper.rawImps(10)).isEqualTo(0);
    assertThat(Imper.rawImps(20)).isEqualTo(1);
    assertThat(Imper.rawImps(40)).isEqualTo(1);
    assertThat(Imper.rawImps(50)).isEqualTo(2);
    assertThat(Imper.rawImps(80)).isEqualTo(2);
    assertThat(Imper.rawImps(90)).isEqualTo(3);
    assertThat(Imper.rawImps(100)).isEqualTo(3);

    assertThat(Imper.rawImps(150)).isEqualTo(4);
    assertThat(Imper.rawImps(1200)).isEqualTo(15);

    // negative scores are symmetric
    assertThat(Imper.rawImps(-10)).isEqualTo(0);
    assertThat(Imper.rawImps(-20)).isEqualTo(-1);
    assertThat(Imper.rawImps(-40)).isEqualTo(-1);
    assertThat(Imper.rawImps(-50)).isEqualTo(-2);
    assertThat(Imper.rawImps(-80)).isEqualTo(-2);
    assertThat(Imper.rawImps(-90)).isEqualTo(-3);
    assertThat(Imper.rawImps(-100)).isEqualTo(-3);

    assertThat(Imper.rawImps(-150)).isEqualTo(-4);
    assertThat(Imper.rawImps(-1200)).isEqualTo(-15);
  }
}
