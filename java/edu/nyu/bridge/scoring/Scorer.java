package edu.nyu.bridge.scoring;

import static com.google.common.base.Preconditions.checkArgument;
import static edu.nyu.bridge.util.Vulnerabilities.isVul;

import edu.nyu.bridge.gen.Bridge;
import edu.nyu.bridge.gen.Bridge.Vulnerability;
import edu.nyu.bridge.util.Bids;
import edu.nyu.bridge.util.Contract;
import edu.nyu.bridge.util.Contract.Bonus;
import edu.nyu.cards.gen.Cards;

public final class Scorer {
  private Scorer() {}

  private static int[] NON_VUL_DOUBLED = {100, 300, 500};
  private static int[] VUL_DOUBLED = {200, 500};

  /** Returns NS-relative score. */
  public static Score score(Contract contract, Vulnerability vul, Result result) {
    if (contract.isPassout()) {
      return Score.of(Bridge.Direction.NORTH, 0);
    }
    if (result.getContractTricks() < contract.getContractTricks()) {
      return Score.of(contract.getDeclarer(), -undertricks(contract, vul, result));
    }
    int trickScore = trickScore(contract) * contract.getCall().getNonBid().getNumber();
    boolean isVul = isVul(contract.getDeclarer(), vul);
    return Score.of(
        contract.getDeclarer(),
        trickScore
            + insult(contract)
            + overtrickScore(
                contract, result.getContractTricks() - contract.getContractTricks(), isVul)
            + makingBonus(contract, trickScore, isVul));
  }

  private static int insult(Contract contract) {
    switch (contract.getCall().getNonBid()) {
      case REDOUBLE:
        return 100;
      case DOUBLE:
        return 50;
      default:
        return 0;
    }
  }

  private static int makingBonus(Contract contract, int trickScore, boolean isVul) {
    Bonus bonus = bonus(contract, trickScore);
    return isVul ? bonus.vulBonus : bonus.nonVulBonus;
  }

  private static int overtrickScore(Contract contract, int overtricks, boolean isVul) {
    checkArgument(overtricks >= 0, "overtricks %s should not be neg", overtricks);
    int doubledTrick = isVul ? 200 : 100;
    switch (contract.getCall().getNonBid()) {
      case REDOUBLE:
        return overtricks * 2 * doubledTrick;
      case DOUBLE:
        return overtricks * doubledTrick;
      default:
        break;
    }
    Cards.Suit trump = Bids.suit(contract.getCall().getBid());
    switch (trump) {
      case NOTRUMPS:
      case SPADES:
      case HEARTS:
        return 30 * overtricks;
      case CLUBS:
      case DIAMONDS:
        return 20 * overtricks;
      default:
        throw new IllegalArgumentException("contract=" + contract);
    }
  }

  private static int trickScore(Contract contract) {
    Cards.Suit trump = Bids.suit(contract.getCall().getBid());
    switch (trump) {
      case NOTRUMPS:
        return 10 + 30 * contract.getContractTricks();
      case SPADES:
      case HEARTS:
        return 30 * contract.getContractTricks();
      case CLUBS:
      case DIAMONDS:
        return 20 * contract.getContractTricks();
      default:
        throw new IllegalArgumentException("contract=" + contract);
    }
  }

  private static int undertricks(Contract contract, Vulnerability vulnerability, Result result) {
    // Bridge.NonBid risk, Vulnerability vul, int number) {
    int down = contract.getContractTricks() - result.getContractTricks();
    checkArgument(down > 0, "expected positive undertricks down=%s", down);
    boolean vul = isVul(contract.getDeclarer(), vulnerability);
    switch (contract.getCall().getNonBid()) {
      case PASS:
        return down * (vul ? 100 : 50);
      case DOUBLE:
        return doubledScore(vul ? VUL_DOUBLED : NON_VUL_DOUBLED, down);
      case REDOUBLE:
        return 2 * doubledScore(vul ? VUL_DOUBLED : NON_VUL_DOUBLED, down);
    }
    throw new IllegalArgumentException("Unexpected contract risk: " + contract.getCall());
  }

  private static int doubledScore(int[] starter, int down) {
    if (down < starter.length) {
      return starter[down - 1];
    }
    return starter[starter.length - 1] + (down - starter.length) * 300;
  }

  public static Bonus bonus(Contract contract, int trickScore) {
    if (contract.isGrand()) {
      return Bonus.GRAND;
    }
    if (contract.isSlam()) {
      return Bonus.SLAM;
    }
    return trickScore < 100 ? Bonus.PARTSCORE : Bonus.GAME;
  }
}
