package edu.nyu.bridge.util;

import static com.google.common.truth.Truth.assertThat;

import edu.nyu.bridge.gen.Bridge.Vulnerability;
import org.junit.Test;

public class DealsTest {
  @Test
  public void testVuls() {
    assertThat(Vulnerability.NONE).isEqualTo(Deals.getVul(8));
    assertThat(Vulnerability.BOTH).isEqualTo(Deals.getVul(4));

    assertThat(Vulnerability.NS).isEqualTo(Deals.getVul(18));
    assertThat(Vulnerability.EW).isEqualTo(Deals.getVul(32));
  }
}
