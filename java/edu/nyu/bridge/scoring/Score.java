package edu.nyu.bridge.scoring;

import static edu.nyu.bridge.util.Directions.isNS;

import com.google.auto.value.AutoValue;
import com.google.common.base.Objects;
import com.google.errorprone.annotations.Immutable;
import edu.nyu.bridge.gen.Bridge.Direction;

/** Represents a score with Duplicate bonuses. */
@AutoValue
@Immutable
public abstract class Score {
  public static Score of(Direction direction, int score) {
    return new AutoValue_Score(direction, score);
  }

  /** Should be N or E to represent NS or EW respectively. */
  public abstract Direction getDirection();

  /** Score relative to direction. */
  public abstract int getScore();

  /** Score always NS relative so if EW make 4H white, score is -420. */
  public Score absolute() {
    if (isNS(getDirection())) {
      return this;
    }
    return of(Direction.NORTH, -getScore());
  }

  @Override
  public boolean equals(Object other) {
    if (!(other instanceof Score)) {
      return false;
    }
    Score otherScore = (Score) other;
    int thisScore = isNS(getDirection()) ? getScore() : -getScore();
    int thatScore =
        isNS(otherScore.getDirection()) ? otherScore.getScore() : -otherScore.getScore();
    return thisScore == thatScore;
  }

  @Override
  public int hashCode() {
    Score abs = this.absolute();
    return Objects.hashCode(abs.getDirection(), abs.getScore());
  }
}
