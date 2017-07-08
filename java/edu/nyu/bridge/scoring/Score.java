package edu.nyu.bridge.scoring;

import com.google.auto.value.AutoValue;
import edu.nyu.bridge.gen.Bridge.Direction;
import edu.nyu.bridge.util.Directions;

/** Represents a score with Duplicate bonuses. */
@AutoValue
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
    if (Directions.isNS(getDirection())) {
      return this;
    }
    return of(Direction.NORTH, -getScore());
  }
}
