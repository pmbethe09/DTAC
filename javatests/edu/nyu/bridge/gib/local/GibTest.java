package edu.nyu.bridge.gib.local;

import static com.google.common.truth.Truth.assertThat;

import com.google.common.io.ByteStreams;
import com.google.inject.util.Providers;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.player.HandPlayer;
import edu.nyu.bridge.util.Calls;
import edu.nyu.cards.Hand;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import org.junit.Test;

public class GibTest {
  @Test
  public void testLog() throws IOException {
    try (HandPlayer hp =
        new LocalGibPlayer(Providers.of(new TextSession()))
            .newDeal(Hand.fromString("AKQ.KQJ.QJT.JT98").toProto(), 1, Direction.SOUTH)) {
      hp.nextBid(Calls.withDescription(Calls.PASS));
      hp.nextBid(Calls.withDescription(Calls.PASS));
      assertThat(hp.makeBid()).named("open 1C").isEqualTo(Calls.string2Call("1C"));
    }
  }

  private static class TextSession implements LocalGibPlayer.Session {
    @Override
    public void close() {}

    @Override
    public InputStream getInputStream() {
      return new ByteArrayInputStream(GIB.getBytes());
    }

    @Override
    public OutputStream getOutputStream() {
      return ByteStreams.nullOutputStream();
    }
  }

  private static final String GIB =
      "GIB version 6.2.0, 11/18/08\n\n"
          + "Enter S hand: "
          + "Enter dealer: "
          + "Enter vul (none, N/S, E/W, both): "
          + "Enter bid for N: "
          + "That bid shows: No suitable call -- 11- HCP; 12- total points\n"
          + "Enter bid for E: "
          + "That bid shows: No suitable call -- 11- HCP; 12- total points\n"
          + "I bid 1C\n";
}
