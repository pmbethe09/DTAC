package edu.nyu.bridge.util;

import static edu.nyu.bridge.util.Directions.add;
import static edu.nyu.bridge.util.Directions.human;
import static edu.nyu.bridge.util.Directions.lho;
import static edu.nyu.bridge.util.Directions.opponents;
import static edu.nyu.bridge.util.Directions.offset;
import static edu.nyu.bridge.util.Directions.partner;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import edu.nyu.bridge.gen.Bridge.Direction;

public class DirectionsTest {
  @Test
  public void testBasic() {
    assertEquals("South", human(Direction.SOUTH));
    assertEquals(Direction.EAST, lho(Direction.NORTH));
    assertEquals("ring around", Direction.NORTH, lho(lho(partner(Direction.NORTH))));
  }

  @Test
  public void testAdd() {
    assertEquals("5 bids", lho(Direction.SOUTH), add(Direction.SOUTH, 5));
    assertEquals("12 bids", Direction.WEST, add(Direction.WEST, 12));
  }

  @Test
  public void testOpponents() {
    assertTrue(opponents(Direction.SOUTH, Direction.WEST));
    assertFalse(opponents(Direction.SOUTH, Direction.NORTH));
  }

  @Test
  public void testOffset() {
    assertEquals("lho 1", 1, offset(Direction.SOUTH, Direction.WEST));
  }
}
