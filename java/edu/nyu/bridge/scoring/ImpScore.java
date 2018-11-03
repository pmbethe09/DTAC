package edu.nyu.bridge.scoring;

import com.google.auto.value.AutoValue;
import com.google.errorprone.annotations.Immutable;
import edu.nyu.bridge.gen.Bridge.Direction;

/**
 * Represents an imp score for a particular board.
 *
 * <p>Re-uses an underlying {@link Score}, but wrapped this way so that the types are not considered
 * equivalent.
 */
@AutoValue
@Immutable
public abstract class ImpScore {
  public static ImpScore of(Score score) {
    return new AutoValue_ImpScore(score);
  }

  public static ImpScore of(Direction direction, int score) {
    return new AutoValue_ImpScore(Score.of(direction, score));
  }

  public abstract Score score();

  /** Absolute score from the NS direction. */
  public ImpScore absolute() {
    Score abs = score().absolute();
    if (abs == score()) {
      return this;
    }
    return of(abs);
  }

  public ImpScore plus(ImpScore next) {
    return add(this, next);
  }

  public static ImpScore add(ImpScore left, ImpScore right) {
    if (left.score().getScore() == 0) {
      return right;
    }
    if (right.score().getScore() == 0) {
      return left;
    }
    int leftAbs = left.score().absolute().getScore();
    int rightAbs = right.score().absolute().getScore();
    return of(Score.of(Direction.NORTH, leftAbs + rightAbs));
  }
}
