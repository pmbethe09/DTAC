package edu.nyu.bridge.util;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class CallsTest {
  @Test
  public void testThereAndBackAgain() {
    assertEquals("2N", Calls.call2String(Calls.string2Call("2N")));
  }

  @Test
  public void testBidCompare() {
    assertTrue(Calls.allowed(Calls.string2Call("2N"), Calls.string2Call("3C")));
    assertTrue(Calls.allowed(Calls.PASS, Calls.string2Call("3C")));
    assertTrue(Calls.allowed(Calls.PASS, Calls.PASS));
    assertTrue(Calls.allowed(Calls.string2Call("2N"), Calls.PASS));

    assertFalse(Calls.allowed(Calls.string2Call("2N"), Calls.string2Call("2N")));
    assertFalse(Calls.allowed(Calls.string2Call("2N"), Calls.string2Call("2S")));
    assertFalse(Calls.allowed(Calls.string2Call("2D"), Calls.string2Call("1N")));
  }
}
