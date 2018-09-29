package edu.nyu.bridge.util;

import static com.google.common.truth.Truth.assertThat;
import static edu.nyu.bridge.util.Directions.add;
import static edu.nyu.bridge.util.Directions.human;
import static edu.nyu.bridge.util.Directions.lho;
import static edu.nyu.bridge.util.Directions.offset;
import static edu.nyu.bridge.util.Directions.opponents;
import static edu.nyu.bridge.util.Directions.partner;

import com.google.common.collect.ImmutableList;
import edu.nyu.bridge.gen.Bridge.Direction;
import org.junit.Test;

public class DirectionsTest {
  @Test
  public void testBasic() {
    assertThat("South").isEqualTo(human(Direction.SOUTH));
    assertThat(Direction.EAST).isEqualTo(lho(Direction.NORTH));
    assertThat(Direction.NORTH).named("ring around").isEqualTo(lho(lho(partner(Direction.NORTH))));
  }

  @Test
  public void testAdd() {
    assertThat(lho(Direction.SOUTH)).named("5 bids").isEqualTo(add(Direction.SOUTH, 5));
    assertThat(Direction.WEST).named("12 bids").isEqualTo(add(Direction.WEST, 12));
  }

  @Test
  public void testOpponents() {
    assertThat(opponents(Direction.SOUTH, Direction.WEST)).isTrue();
    assertThat(opponents(Direction.SOUTH, Direction.NORTH)).isFalse();
  }

  @Test
  public void testOffset() {
    assertThat(1).named("lho 1").isEqualTo(offset(Direction.SOUTH, Direction.WEST));
  }

  @Test
  public void directionsMap() {
    for (Direction direction : Direction.values()) {
      ImmutableList<Direction> directionList = Directions.iterateDirections(direction);
      assertThat(directionList.get(0)).isEqualTo(direction);
      Direction previous = direction;
      for (Direction fromList : directionList) {
        assertThat(fromList).isEqualTo(previous);
        previous = Directions.lho(fromList);
      }
    }
  }
}
