package edu.nyu.bridge.scoring;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.bridge.gen.Bridge.Direction;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public class ScoreTest {
  @Test
  public void testAbsoluteEquals() {
    assertThat(Score.of(Direction.EAST, 420)).isEqualTo(Score.of(Direction.NORTH, -420));
    assertThat(Score.of(Direction.NORTH, -620)).isEqualTo(Score.of(Direction.WEST, 620));
    assertThat(Score.of(Direction.NORTH, -620)).isNotEqualTo(Score.of(Direction.NORTH, 620));
    assertThat(Score.of(Direction.EAST, 420)).isNotEqualTo(Score.of(Direction.EAST, -420));
  }
}
