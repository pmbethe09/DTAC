package edu.nyu.bridge.gib.local;

import static java.lang.annotation.RetentionPolicy.RUNTIME;

import com.google.inject.BindingAnnotation;
import java.lang.annotation.Retention;

public final class Annotations {
  @BindingAnnotation
  @Retention(value = RUNTIME)
  public static @interface Gib {}
}
