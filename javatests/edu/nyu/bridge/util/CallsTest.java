package edu.nyu.bridge.util;

import static com.google.common.truth.Truth.assertThat;

import org.junit.Test;

public class CallsTest {
  @Test
  public void testThereAndBackAgain() {
    assertThat(Calls.call2String(Calls.string2Call("2N"))).isEqualTo("2N");
  }

  @Test
  public void testBidCompare() {
    assertThat(Calls.allowed(Calls.string2Call("2N"), Calls.string2Call("3C"))).isTrue();
    assertThat(Calls.allowed(Calls.PASS, Calls.string2Call("3C"))).isTrue();
    assertThat(Calls.allowed(Calls.PASS, Calls.PASS)).isTrue();
    assertThat(Calls.allowed(Calls.string2Call("2N"), Calls.PASS)).isTrue();

    assertThat(Calls.allowed(Calls.string2Call("2N"), Calls.string2Call("2N"))).isFalse();
    assertThat(Calls.allowed(Calls.string2Call("2N"), Calls.string2Call("2S"))).isFalse();
    assertThat(Calls.allowed(Calls.string2Call("2D"), Calls.string2Call("1N"))).isFalse();
  }
}
