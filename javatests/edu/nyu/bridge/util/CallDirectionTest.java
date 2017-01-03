package edu.nyu.bridge.util;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import com.google.common.base.Converter;

import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.util.ShortEnumConverter;

public class CallDirectionTest {
  @Test
  public void testCalls() {
    assertEquals("2-ways", "2H", Calls.call2String(Calls.string2Call("2H")));
  }

  @Test
  public void testDirectionConverter() {
    Converter<String, Direction> directionConverter = ShortEnumConverter.create(Direction.class);
    assertEquals("double convert", "N",
        directionConverter.reverse().convert(directionConverter.convert("N")));
  }

  @Test
  public void testDirections() {
    assertEquals("compass", Direction.EAST, Directions.lho(Direction.NORTH));
  }
}
