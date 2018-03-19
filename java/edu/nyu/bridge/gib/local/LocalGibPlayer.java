package edu.nyu.bridge.gib.local;

import static com.google.common.base.Preconditions.checkState;
import static edu.nyu.cards.Cards.card2String;
import static edu.nyu.cards.Cards.string2Card;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.List;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.inject.Inject;

import com.google.inject.Provider;

import edu.nyu.bridge.gen.Bridge.Call;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.gen.Bridge.Vulnerability;
import edu.nyu.bridge.player.BridgePlayer;
import edu.nyu.bridge.player.HandPlayer;
import edu.nyu.bridge.scoring.Result;
import edu.nyu.bridge.util.Calls;
import edu.nyu.bridge.util.Deals;
import edu.nyu.bridge.util.Directions;
import edu.nyu.cards.Hand;
import edu.nyu.cards.gen.Cards;
import edu.nyu.cards.gen.Cards.Card;

/**
 * Implements the {@link BridgePlayer}/{@link HandPlayer} protocols
 * while using an underlying commandline GiB session to determine moves.
 */
public class LocalGibPlayer implements BridgePlayer {
  private static final Logger log = Logger.getLogger(LocalGibPlayer.class.getName());
  private static final Pattern I_BID = Pattern.compile("I bid (\\w+)");
  private static final Pattern I_PLAY = Pattern.compile("I play (\\w)(\\w)");

  /** Session with Gib.  Usually commandline but abstracted for testing. */
  public interface Session extends Closeable {
    InputStream getInputStream();
    OutputStream getOutputStream();
  }

  private final Provider<Session> sessionFactory;

  @Inject
  LocalGibPlayer(Provider<Session> sessionFactory) {
    this.sessionFactory = sessionFactory;
  }

  @Override
  public HandPlayer newDeal(Cards.Hand hand, int boardNumber, Direction direction) {
    return new GibHandPlayer(hand, boardNumber, direction);
  }

  /** Local GiB version of {@link HandPlayer} using the commandline {@link Session}. */
  class GibHandPlayer implements HandPlayer {
    private final int offset; // TODO figure out how to set the GIB direction.
    private final int boardNumber;
    private final Session session;
    private final BufferedReader in;
    private final PrintWriter out;

    private boolean inPlay = false;

    GibHandPlayer(Cards.Hand hand, int boardNumber, Direction direction) {
      this.offset = Directions.offset(direction, Direction.SOUTH);
      this.boardNumber = boardNumber;
      this.session = sessionFactory.get();
      in = new BufferedReader(new InputStreamReader(session.getInputStream()));
      out = new PrintWriter(session.getOutputStream(), true);
      // setup: hand, dealer, vul
      readLine(); // Version
      readLine(); // blank
      readTill(':');
      out.println(Hand.fromProto(hand).toString());
      readTill(':');
      out.println(Directions.add(Deals.getDealer(boardNumber), offset));
      readTill(':');
      out.println(getVul());
    }

    private String readLine() {
      try {
        String msg = in.readLine();
        log.info("[gib] " + msg);
        return msg;
      } catch (IOException e) {
        throw new IllegalStateException("Failed to read from GiB", e);
      }
    }

    private char read() {
      try {
        return (char) in.read();
      } catch (IOException e) {
        throw new IllegalStateException("Failed to read from GiB", e);
      }
    }

    private String readTill(char found) {
      StringBuffer result = new StringBuffer();
      char c = read();
      while (c != found) {
        result.append(c);
        c = read();
      }
      log.info("[gib] " + result.toString());
      return result.toString();
    }

    @Override
    public Call makeBid() {
      String s = readLine();
      Matcher m = I_BID.matcher(s);
      checkState(m.find(), "failed to parse bid: %s", s);
      readLine(); // TODO, this is GIB's description of its bid.
      return Calls.withDescription(Calls.string2Call(m.group(1)));
    }

    @Override
    public void nextBid(Call waitForBid) {
      readTill(':');
      out.println(Calls.call2String(waitForBid));
      System.out.println(readLine());
    }

    @Override
    public Card makePlay(List<Card> current) {
      return play();
    }

    private Card play() {
      checkPlayTransition();
      String s = readLine();
      if (s.trim().isEmpty()) {
        s = readLine(); // sometimes GiB adds an extra newline
      }
      Matcher m = I_PLAY.matcher(s);
      checkState(m.find(), "failed to parse play: %s", s);
      return string2Card(m.group(1) + m.group(2));
    }

    @Override
    public Card playDummy(List<Card> current) {
      return play();
    }

    @Override
    public void nextPlay(Card card) {
      checkPlayTransition();
      readTill(':');
      out.println(card2String(card));
    }

    private void checkPlayTransition() {
      if (inPlay) {
        return;
      }
      inPlay = true;
      readLine();
    }

    @Override
    public void dummy(Cards.Hand hand) {
      readTill(':');
      out.println(Hand.fromProto(hand).toString());
    }

    @Override
    public boolean doesAcceptClaim(
        Cards.Hand claimer, Result totalDeclarerTricks, String description) {
      return false;
    }

    private String getVul() {
      Vulnerability vul = Deals.getVul(boardNumber);
      if (offset == 1 || offset == 3) { // then flip
        if (vul == Vulnerability.EW) {
          vul = Vulnerability.NS;
        } else if (vul == Vulnerability.NS) {
          vul = Vulnerability.EW;
        }
      }
      switch (vul) {
        case BOTH:
          return "both";
        case EW:
          return "E/W";
        case NONE:
          return "none";
        case NS:
          return "N/S";
        default:
          throw new IllegalArgumentException("unexpected vul: " + vul);
      }
    }

    @Override
    public void close() throws IOException {
      session.close();
    }
  }
}
